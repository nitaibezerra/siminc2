<?php

class ValorMensalObra extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.valormensalobra";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "vmoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'vmoid'		=> null,
									  	'obrid'	    => null,
									  	'vmovalor'	=> null,
                                        'vmomes'    => null,
                                        'vmoano'    => null
									  );


    public function calculaCronograma($obrid){
        /**
         *    obras2.obras
         *      portos.estruturaobra => X
         *			obras2.itenscomposicaoobra
         *              obras2.detalheitem
         *                  obras2.previsaodetalheitem
         */

        $arValoresMensal = array();

        $sqlObra = "SELECT
                       obrid            AS id,
                       ocrvalorexecucao AS valor,
                       ocrdtinicioexecucao AS inicio,
                       ocrdtterminoexecucao AS fim
                    FROM
                       obras2.obrascontrato
                    WHERE
                       ocrstatus = 'A' AND
                       obrid = {$obrid};";

        $dadosObra = $this->pegaLinha($sqlObra);

//        $sqlEstruturaObra = "SELECT
//                                eobid      AS id,
//                                eobvalor   AS valor,
//                                eobinicio  AS inicio,
//                                eobtermino AS fim
//                             FROM
//                                portos.estruturaobra
//                             WHERE
//                                obrid = {$obrid} AND eobstatus = 'A';";
//
//        $dadosEstruturasObra = $this->carregar($sqlEstruturaObra);
//        $subtrairEstruturasObra = 0;
//         se obra possui estrutura
//        if( is_array($dadosEstruturasObra) ){
//            foreach ($dadosEstruturasObra as $estruturaObra){
//                $subtrairEstruturasObra += $estruturaObra['valor'];
                $sqlItensComposicao = "SELECT
                                          i.icoid           AS id,
                                          i.icovlritem      AS valor,
                                          i.icodtinicioitem AS inicio,
                                          i.icodterminoitem AS fim
                                       FROM
                                          obras2.itenscomposicaoobra i
                                          JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus = 'A'
                                       WHERE
                                          cro.obrid = {$obrid} AND
                                          i.icostatus = 'A';";
                $dadosItensComposicao = $this->carregar($sqlItensComposicao);

                $subtrairItensComposicao = 0;
                // se estrutura possui itens composição
                if( is_array($dadosItensComposicao) ){
                    foreach ($dadosItensComposicao as $itemComposicao){
                        $subtrairItensComposicao += $itemComposicao['valor'];

                        $sqlDetalheItem = "SELECT
                                              ditid          AS id,
                                              ditvalor       AS valor,
                                              ditdatainicio  AS inicio,
                                              ditdatatermino AS fim
                                           FROM
                                              obras2.detalheitem
                                           WHERE
                                              icoid = {$itemComposicao['id']} AND 
                                              ditstatus = 'A' AND 
                                              NOT ditidpai IS NULL;";
                        $dadosDetalheItem = $this->carregar($sqlDetalheItem);
                        $dadosDetalheItem = is_array($dadosDetalheItem) ? $dadosDetalheItem : array();

                        // itens composicao sem filhos
                        $sqlDetalheItemSemFilho = "SELECT
                                                      ditid          AS id,
                                                      ditvalor       AS valor,
                                                      ditdatainicio  AS inicio,
                                                      ditdatatermino AS fim
                                                   FROM
                                                      obras2.detalheitem
                                                   WHERE
                                                      icoid = {$itemComposicao['id']} AND ditstatus = 'A'
                                                                                      AND ditidpai IS NULL
                                                                                      AND ditid NOT IN  (SELECT DISTINCT
                                                                                                            COALESCE(ditidpai, -1)
                                                                                                         FROM
                                                                                                            obras2.detalheitem
                                                                                                         WHERE
                                                                                                            icoid = {$itemComposicao['id']} AND ditstatus = 'A');";

                        $dadosDetalheItemSemFilho = $this->carregar($sqlDetalheItemSemFilho);
                        $dadosDetalheItemSemFilho = is_array($dadosDetalheItemSemFilho) ? $dadosDetalheItemSemFilho : array();
                        $dadosDetalheItem         = array_merge($dadosDetalheItem, $dadosDetalheItemSemFilho);

                        $subtrairDetalheItem = 0;
                        // se itens composição possui detalhe item
                        if( is_array($dadosDetalheItem) ){
                            foreach ($dadosDetalheItem as $detalheItem){
                                $subtrairDetalheItem += $detalheItem['valor'];

                                $sqlPrevisaoDetalheItem = "SELECT
                                                              pdiid    AS id,
                                                              pdivalor AS valor,
                                                              pdimes   AS data
                                                           FROM
                                                              obras2.previsaodetalheitem
                                                           WHERE
                                                              ditid = {$detalheItem['id']} AND pdistatus = 'A';";
                                $dadosPrevisaoDetalheItem = $this->carregar($sqlPrevisaoDetalheItem);

                                $subtrairPrevisaoDetalheItem = 0;
                                // se detalhe item possui previsao
                                if( is_array($dadosPrevisaoDetalheItem) ){
                                    foreach ($dadosPrevisaoDetalheItem as $previsaoDetalheItem){
                                        list($ano, $mes) = explode('-', $previsaoDetalheItem['data']);
                                        $ano = intval($ano);
                                        $mes = intval($mes);
                                        if( !isset($arValoresMensal[$ano][$mes]) )
                                            $arValoresMensal[$ano][$mes] = 0;
                                        $arValoresMensal[$ano][$mes] += $previsaoDetalheItem['valor'];
                                        $subtrairPrevisaoDetalheItem += $previsaoDetalheItem['valor'];
                                    }
                                } // FIM se detalhe item possui previsao
                                // processa detalhe item
                                $arValoresMensal = $this->_processaValorPorMes($arValoresMensal, $detalheItem['inicio'], $detalheItem['fim'], $detalheItem['valor'], $subtrairPrevisaoDetalheItem);

                            }
                        } // FIM se itens composição possui detalhe item

                        // processa item composicao
                        $arValoresMensal = $this->_processaValorPorMes($arValoresMensal, $itemComposicao['inicio'], $itemComposicao['fim'], $itemComposicao['valor'], $subtrairDetalheItem);
                    }
                } // FIM se estrutura possui itens composição

                // processa estrutura obra
//                $arValoresMensal = $this->_processaValorPorMes($arValoresMensal, $estruturaObra['inicio'], $estruturaObra['fim'], $estruturaObra['valor'], $subtrairItensComposicao);

//            }
//        } // FIM se obra possui estrutura

        // processa obra
        $arValoresMensal = $this->_processaValorPorMes($arValoresMensal, $dadosObra['inicio'], $dadosObra['fim'], $dadosObra['valor'], $subtrairItensComposicao);
//        $arValoresMensal = $this->_processaValorPorMes($arValoresMensal, $dadosObra['inicio'], $dadosObra['fim'], $dadosObra['valor'], $subtrairEstruturasObra);
        // retira as posições com valores zero
        foreach ($arValoresMensal as $ano => $valorPorAno){
            foreach ($valorPorAno as $mes => $valorPorMes){
                if( $valorPorMes <= 0 || $ano == 0 || $mes == 0){
                    unset($arValoresMensal[$ano][$mes]);
                }
            }
        }

        $this->deleteByObra($obrid);

        foreach ($arValoresMensal as $ano => $valoresAno){
            foreach ($valoresAno as $mes => $valorMes){
                $arDados             = array();
                $arDados['obrid']    = $obrid;
                $arDados['vmovalor'] = $valorMes;
                $arDados['vmomes']   = $mes;
                $arDados['vmoano']   = $ano;

                $this->popularDadosObjeto($arDados);
                $this->salvar();
                $this->clearDados();
            }
        }
        $this->commit();
    }

    public function deleteByObra($obrid){
        $sql = "DELETE FROM obras2.valormensalobra WHERE obrid = {$obrid};";
        $this->executar($sql);
    }

    protected function _diferencaMeses($inicio, $fim){
        $inicio = strtotime($inicio);
        $fim    = strtotime($fim);
        $dif    = ($fim - $inicio) / 86400;
        $meses  = round($dif/30);
        if($meses < 0 )
            $meses = 0;
        return floatval($meses);
    }

    protected function _processaValorPorMes($arValoresMensal, $dataInicio, $dataFim, $valorTotal, $valorSubtrair){
        $valorTotal    = floatval($valorTotal);
        $valorSubtrair = floatval($valorSubtrair);

        $inicio      = explode('-', $dataInicio);
        $inicioMes   = intval($inicio[1]);
        $inicioAno   = intval($inicio[0]);

        $fim         = explode('-', $dataFim);
        $fimMes      = intval($fim[1]);
        $fimAno      = intval($fim[0]);
        $valor       = $valorTotal - $valorSubtrair;

        $meses       = $this->_diferencaMeses($dataInicio, $dataFim);
        $meses       = $meses == 0 ? 1 : $meses;
        $valorPorMes = round($valor / $meses, 2);
        
        for ($meses2 = 0; $meses2 < $meses; $meses2++){
             if( !isset($arValoresMensal[$inicioAno][$inicioMes]) )
                 $arValoresMensal[$inicioAno][$inicioMes] = 0;

            $arValoresMensal[$inicioAno][$inicioMes] += floatval($valorPorMes);

             $inicioMes++;
             if($inicioMes > 12){
                 $inicioMes = 1;
                 $inicioAno++;
             }
        }

        return $arValoresMensal;
    }

    public function getValorMensalByObra($obrid){

        $sql = "SELECT
                    vmo.vmoano AS ano,
                    vmo.vmomes AS mes,
                    vmo.vmovalor AS valorprevisto,
                    (CASE WHEN oc.ocrvalorexecucao <> 0 THEN 
                        vmo.vmovalor / oc.ocrvalorexecucao * 100::numeric 
                        ELSE 0 
                     END) AS percentualprevistomes,
                    oc.ocrvalorexecucao
                FROM
                    obras2.valormensalobra vmo
                INNER JOIN
                    obras2.obrascontrato oc ON oc.obrid = vmo.obrid AND oc.ocrstatus = 'A'
                WHERE
                    vmo.obrid = {$obrid}
                ORDER BY
                    ano, mes";

//        $arPrevisto = $this->carregar($sql);

//        if($arPrevisto === false){
            $this->calculaCronograma($obrid);
            $arPrevisto = $this->carregar($sql);
//        }
        return $arPrevisto;
    }
}