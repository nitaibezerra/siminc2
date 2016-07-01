<?php
	
class SupervisaoItem extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisaoitem";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "spiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'spiid' => null, 
                                        'supid' => null, 
                                        'icoid' => null, 
                                        'ditid' => null, 
                                        'spivlrinfsupervisor' => null, 
                                        'spivlrfinanceiroanterior' => null, 
                                        'spivlritemexecanterior' => null, 
                                        'spivlritemsobreobraexecanterior' => null, 
                                        'spivlrfinanceiroinfsupervisor' => null, 
                                        'spivlrmetafisicasupervisor' => null,
                                        'spitotitemsupervisor' => null,
                                        'spitotitemanterior' => null,
                                      );
/*
//    public function getItensByEstrutura($atiid, $supid){
//        $supid = ($supid ? $supid : 0);
//
//        $sql = "SELECT
//                    sic.*,
//                    ((e.eobvalor * 100)/ati.ativalor) AS eobpercentual,
//                    e.*,
//                    ati.ativalor
//                FROM
//                    obras2.estruturaobra e
//                JOIN
//                    pde.atividade ati ON ati.atiid = e.atiid
//                LEFT JOIN obras2.supervisaoitenscomposicao sic ON sic.eobid = e.eobid
//                                                                 AND sic.supvid = {$supid}
//                                                                 AND sic.eobid IS NOT NULL
//                                                                 AND sic.iceid IS NULL
//                                                                 AND sic.ditid IS NULL
//                WHERE
//                    e.eobstatus = 'A' AND
//                    e.atiid = {$atiid}
//                ORDER BY
//                    e.eobordem";
//
//        $dados = $this->carregar($sql);
//
//        return is_array( $dados ) ? $dados : array();
//    }
*/
    
    public function getItensByEtapa($obrid, $supid, $relativoEdificacao = 'D', $croid = null){
        $supid = ($supid ? $supid : 0);
        $croid = ($croid ? "AND cro.croid = $croid" : 'AND CASE WHEN sup.croid IS NOT NULL THEN cro.croid = sup.croid ELSE cro.crostatus = \'A\' END ');

        $sql = "SELECT
                    sic.*,
                    ic.*,
                    um.umdeesc,
                    CASE 
                    	WHEN ocr.ocrvalorexecucao > 0 THEN
                    		((ico.icovlritem * 100) / ocr.ocrvalorexecucao)
                    	ELSE
                    		0
                    END as icopercsobreobra,
                    ico.*,
                    ocr.ocrvalorexecucao
                FROM obras2.cronograma cro
                JOIN obras2.itenscomposicaoobra ico ON ico.croid = cro.croid AND ico.icostatus = 'A' AND ico.obrid = cro.obrid
                JOIN obras2.itenscomposicao ic ON ico.itcid = ic.itcid AND ic.itcstatus = 'A'

                LEFT JOIN
                    (SELECT 
                    	ocrvalorexecucao, obrid 
                     FROM 
                     	obras2.obrascontrato 
                     WHERE 
                     	obrid = {$obrid} AND 
                     	ocrstatus = 'A'
                        ORDER BY ocrid desc
                        LIMIT 1) ocr ON ocr.obrid = ico.obrid 
                        
                LEFT JOIN ( select * 
                            from obras2.supervisaoitem s1 
                            where s1.spiid in( select max(s2.spiid) 
                                               from obras2.supervisaoitem s2 
                                               where s2.supid = {$supid} 
                                                 and s2.icoid = s1.icoid) 
                            AND s1.supid = {$supid}
                ) sic ON sic.icoid = ico.icoid
                     AND sic.supid = {$supid}
                     AND sic.icoid IS NOT NULL
                     AND sic.ditid IS NULL
                LEFT JOIN obras2.supervisao sup ON sup.supid = {$supid}
                LEFT JOIN obras2.unidademedida um ON um.umdid = ico.umdid

                WHERE
                    cro.crostatus != 'I' AND
                    cro.obrid = {$obrid} AND
                    ico.obrid = cro.obrid AND
                    ico.relativoedificacao = '{$relativoEdificacao}'
                    $croid
                ORDER BY
                    ico.icoordem";

        $dados = $this->carregar($sql);

        // Altera o valor das obras MI para pegar o valor total = a soma dos itens do cronograma
        $obra = new Obras($obrid);
        if($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C){
            $total = 0;
            if(!$dados) return array();
            foreach($dados as $etapa){
                if($relativoEdificacao == 'F')
                    $total += ($etapa['icovlritem'] * $etapa['itcquantidade']);
                else
                    $total += $etapa['icovlritem'];
            }

            foreach($dados as $key => $etapa){
                $dados[$key]['ocrvalorexecucao'] = $total;
            }
        }

        return is_array( $dados ) ? $dados : array();
    }

    public function getItensByDetalhamento($obrid, $icoid, $supid, $where = array()){
        $supid = ($supid ? $supid : 0);
        $where[] = "di.ditstatus = 'A'";

        $sql = "SELECT
                    sic.*,
                    ((di.ditvalor * 100)/ocr.ocrvalorexecucao) as ditpercsobreobra,
                    di.*,
                    umc.umcdsc,
                    ocr.ocrvalorexecucao
                FROM
                    obras2.detalheitem di
                JOIN obras2.itenscomposicaoobra ico ON ico.icoid = di.icoid
                                                       AND ico.icostatus = 'A'
                JOIN
                    (SELECT 
                    	ocrvalorexecucao, obrid 
                     FROM 
                     	obras2.obrascontrato 
                     WHERE 
                     	obrid = {$obrid} AND 
                     	ocrstatus = 'A') ocr ON ocr.obrid = ico.obrid 
                LEFT JOIN obras2.supervisaoitem sic ON sic.ditid = di.ditid
                                                                  AND sic.supid = {$supid}
                                                                  AND sic.icoid IS NULL
                                                                  AND sic.ditid IS NOT NULL
                JOIN obras2.cronograma cro ON ico.croid = cro.croid AND cro.crostatus IN ('A') AND cro.obrid = ico.croid
                LEFT JOIN obras2.unidademedidacronograma umc ON umc.umcid = di.umcid
                                                                AND umcstatus = 'A'
                WHERE
                    di.icoid = {$icoid} AND
                    ".implode(" AND ", $where)."
                ORDER BY
                    di.ditordem";
        
        $dados = $this->carregar($sql);

        return is_array( $dados ) ? $dados : array();
    }
									  
}