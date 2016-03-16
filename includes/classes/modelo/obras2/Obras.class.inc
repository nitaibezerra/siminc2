<?php

class Obras extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.obras";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("obrid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'obrid' => null,
        'endid' => null,
        'empid' => null,
        'iexid' => null,
        'entid' => null,
        'tobid' => null,
        'tpoid' => null,
        'cloid' => null,
        'tooid' => null,
        'arqid' => null,
        'staid' => null,
        'docid' => null,
        'preid' => null,
        'obridpai' => null,
        'obridvinculado' => null,
        'obrnome' => null,
        'obrstatusinauguracao' => null,
        'obrdsc' => null,
        'obrdtinicio' => null,
        'obrdtfim' => null,
        'obrvalorprevisto' => null,
        'obrdtvistoria' => null,
        'obrstatus' => null,
        'numconvenio' => null,
        'obrnumprocessoconv' => null,
        'obranoconvenio' => null,
        'frpid' => null,
        'obrdtultvistoria' => null,
        'obrdtinclusao' => null,
        'obrdtcarga' => null,
        'obrpercentultvistoria' => null,
        'obrperccontratoanterior' => null,
        'obrcronogramaservicocontratado' => null,
        'obrcronogramaservicocontratadojustificativa' => null,
        'obrsndocumentos' => null,
        'obrsnfotos' => null,
        'obrid_1' => null,
        'obrtravaedicaocronograma' => null,
        'usucpfinclusao' => null,
        'stiid' => null,
        'medidasexcecao' => null,
        'strid' => null,
        'obrorgcontrole' => null,
        'obrcontabloqueada' => null,
        'obrprocessoanterior' => null,
    );

    public function pegaDadosCompletoPorObrid($obrid)
    {
        
        if($obrid == NULL || trim($obrid) == ''){
            $obrid = $_SESSION['obras2']['obrid'];
            if($obrid == NULL || trim($obrid) == ''){
                $obrid = $this->obrid;
            }
            if($obrid == NULL || trim($obrid) == ''){
                return array();
            }
        }
        
        $sql = "SELECT
                        og.*,
                        ent.*,
                        e.*,
                        o.*,
                        tob.tobdesc,
                        p.prfdesc,
                        co.clodsc,
                        tpo.tpodsc,
                        tf.frpdesc
                FROM
                        obras2.obras o
                JOIN obras2.empreendimento e                 ON e.empid   = o.empid
                LEFT JOIN obras2.orgao og                    ON og.orgid  = e.orgid
                LEFT JOIN entidade.entidade ent              ON ent.entid = o.entid
                LEFT JOIN obras2.tipoobra tob                ON tob.tobid = o.tobid
                LEFT JOIN obras2.programafonte p             ON p.prfid   = e.prfid
                LEFT JOIN obras2.classificacaoobra co        ON co.cloid  = o.cloid
                LEFT JOIN obras2.tipologiaobra tpo           ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoformarepasserecursos tf ON tf.frpid  = o.frpid
                WHERE
                        o.obrid = {$obrid}";
        $dados = $this->pegaLinha($sql);

        return ($dados ? $dados : array());
    }

    public function listaUnidadesObrasSQL(Array $param = array(), $tipoSelecao = null)
    {

        $where = array();
        $join = array();

        $where = $this->filtroSql($param, $tipoSelecao);
        $join = $this->joinSql($param);

        $sql = "SELECT
                        ent.entid as id,
                        ent.entnome as unidade,
                        count(o.obrid) as total
                FROM
                        entidade.entidade ent
                INNER JOIN obras2.obras 			   o ON    o.entid = ent.entid
                INNER JOIN obras2.empreendimento 	   e ON    e.empid = o.empid
                LEFT  JOIN entidade.entidadeendereco eed ON  eed.entid = ent.entid
                LEFT  JOIN entidade.endereco 		ende ON ende.endid = eed.endid AND
                                                                                                        ende.endstatus = 'A'
                " . (count($join) ? implode(" ", $join) : "") . "
                WHERE
                        " . (count($where) ? implode(' AND ', $where) : "") . "
                GROUP BY
                        ent.entid,
                        ent.entnome";

        return $sql;
    }

    public function listaUnidadesObras()
    {

        $arEntid = str_replace("}{", ",", $_SESSION["obrasarvore"]["arEntid"]);
        $arEntid = str_replace("{", "", $arEntid);
        $arEntid = str_replace("}", "", $arEntid);
        $arEntid = explode(",", $arEntid);

        $sql = $this->listaUnidadesObrasSQL();

        //paginação
        $perpage = 50;
        $pages = 10;
        $numero = $_REQUEST["numero"];
        if (!$numero){
            $numero = 1;
        }

        $sqlCount = "select count(1) from (" . $sql . ") rs";

        $totalRegistro = $this->pegaUm($sqlCount, 0, 3600);

        $sql = $sql . " LIMIT {$perpage} offset " . ($numero - 1);

        $unidades = $this->carregar($sql, null, 3600);

        $nlinhas = count($unidades);
        if (!$unidades)
            $nl = 0;
        else
            $nl = $nlinhas;
        $reg_fim = $nlinhas;
        if ($nl > 0)
            $total_reg = $totalRegistro;
        //fim paginação

        if (is_array($unidades)) {

            print "<script>
                        jQuery(document).ready(function(){
                                jQuery('.listaObras').click(function(){
                                        var entid = jQuery(this).attr('id');
                                        if( jQuery('#trFilhoLista'+entid).css('display') == 'none' ){
                                                if( jQuery('#divFilhoLista'+entid).html() == '' ){
                                                        $.ajax({
                                                                url  		: window.location.href+'&'+jQuery('#formListaObra').serialize(),
                                                                type 		: 'post',
                                                                data 		: {req  : 'listaObrasUnidade',
                                                                                            entid : entid},
                                                                dataType   	: 'html',
                                                                async		: false,
                                                                beforeSend 	: function (){
                                                                        divCarregando();
                                                                },
                                                                error 	 	: function (){
                                                                        divCarregado();
                                                                },
                                                                success	 	: function ( data ){
                                                                        jQuery('#divFilhoLista'+entid).html( data );
                                                                        divCarregado();
                                                                }
                                                        });
                                                }
                                                jQuery('#trFilhoLista'+entid).show();
                                                jQuery(this).attr('src','../imagens/menos.gif');
                                        }else{
                                                jQuery('#trFilhoLista'+entid).hide();
                                                jQuery(this).attr('src','../imagens/mais.gif');
                                        }
                                });
                        });
                   </script>";

            print "<table class='tabela' bgcolor='#ffffff' cellspacing='1' cellpadding='3' align='center'>"
                    . "    <tr>"
                    . "        <td class='SubTituloCentro'>Ação</td>"
                    . "        <td class='SubTituloCentro'>Unidade Implantadora</td>"
                    . "        <td class='SubTituloCentro'>Quantidade de Obras</td>"
                    . "    </tr>";

            $ar = 0;

            for ($i = 0; $i < count($unidades); $i++) {

                $totalObras = $totalObras + $unidades[$i]["total"];

                $cor = ($i % 2) ? "" : "#F7F7F7";

                print "<tr bgcolor='{$cor}'>"
                        . "    <td style='text-align:center;'>"
                        . "        <img src='../imagens/mais.gif' id='{$unidades[$i]["id"]}' style='cursor:pointer;' class=\"listaObras\"/>"
                        . "    </td>"
                        . "    <td>{$unidades[$i]["unidade"]}</td>"
                        . "    <td style='text-align:right; color:#0066cc;'>{$unidades[$i]["total"]}</td>"
                        . "</tr>"
                        . "<tr id='trFilhoLista{$unidades[$i]["id"]}' style='display:none;'>"
                        . "    <td colspan='3'>"
                        . "			 <div id='divFilhoLista{$unidades[$i]["id"]}'></div>"
                        . "    </td>"
                        . "</tr>";
            }

            print "<tr bgcolor='#D0D0D0' style='text-align:right;'>"
                    . "    <td></td>"
                    . "    <td><b>Total de obras</b></td>"
                    . "    <td style='color:#0066cc;'><b>{$totalObras}</b></td>"
                    . "</tr>"
                    . "</table>";

            if ($nl > 0) {
                print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';
                include APPRAIZ . "includes/paginacao.inc";
                print '</td></tr></table>';
                print '<script language="JavaScript">function pagina(numero) {document.formulario.numero.value=numero;document.formulario.submit();}</script>';
            }
        } else {

            print "<table class='tabela' bgcolor='#ffffff' cellspacing='1' cellpadding='3' align='center'>"
                    . "    <tr>"
                    . "    <td style='text-align:center; color:ee0000;'>Não foram encontradas unidades.</td>"
                    . "    </tr>"
                    . "</table>";
        }

        return true;
    }

    public function listaUnidadesObrasExcel()
    {
        global $db;
        $arEntid = str_replace("}{", ",", $_SESSION["obrasarvore"]["arEntid"]);
        $arEntid = str_replace("{", "", $arEntid);
        $arEntid = str_replace("}", "", $arEntid);
        $arEntid = explode(",", $arEntid);

        $sql = $this->listaUnidadesObrasSQL();
        $unidades = $this->carregar($sql, null, 3600);

        $arrExcel = array();
        if (is_array($unidades)) {
            foreach ($unidades as $key => $v) {
                $arrExcel[$key] = array('unidade' => $v['unidade'],
                    'total' => $v['total']
                );
            }
        }
        ob_clean();
        header('content-type: text/html; charset=ISO-8859-1');
        $cabecalho = array('Unidade Implantadora', 'Quantidade de Obras');
        $db->sql_to_xml_excel($arrExcel, 'relatorioListaObjetosUnidades', $cabecalho);
        return true;
    }

    public function listaObrasExcelMi($sql, $cabecalho)
    {
        global $db;

        foreach ($cabecalho as $k => $cab) {
            $cabecalho[$k] = strip_tags($cab);
        }

        $arObras = $this->carregar($sql, null, 3600);
        
        $arrExcel = array();

        if (is_array($arObras)) {
            foreach ($arObras as $key => $v) {
                
                $arrExcel[$key] = array('id'                    => $v['id'],
                                        'preid'                 => $v['preid'],
                                        'pronumeroprocesso'     => $v['pronumeroprocesso'],
                                        'anoprocesso'           => $v['anoprocesso'],
                                        'termo_convenio'        => $v['termo_convenio'],
                                        'ano_termo_convenio'    => $v['ano_termo_convenio'],
                                        'descricao'             => strip_tags($v['descricao']),
                                        'emidsc'                => strip_tags($v['emidsc']),
                                        'unidade_implantadora'  => $v['unidade_implantadora'],
                                        'municipio'             => $v['municipio'],
                                        'bairro'                => $v['bairro'],
                                        'inicio'                => $v['inicio'],
                                        'termino'               => $v['termino'],
                                        'situacao'              => $v['situacao'],
                                        'ultima_atualizacao'    => strip_tags($v['ultima_atualizacao']),
                                        'dtvistoria'            => $v['dtvistoria'],
                                        'obrpercentultvistoria' => $v['obrpercentultvistoria'],
                                        'tpodsc'                => $v['tpodsc'],
                                        'ocrvalorexecucao'      => $v['ocrvalorexecucao'],
                                        'soma_evo'              => $v['soma_evo']
                    /*                    'anexo' => $anexo,
                    //                    'foto' => $foto,
                    //                    'restricoes_superadas' => $restricao['superadas'],
                    //                    'restricoes_pendentes' => $restricao['pendentes'],
                    //                    'restricoes_atrasadas' => $restricao['atraso'],
                    //                    'impedimentos_superados' => $impedimento['superadas'],
                    //                    'impedimentos_pendentes' => $impedimento['pendentes'],
                    //                    'impedimentos_atrasados' => $impedimento['atraso'],
                                        */
                );
            }
        }
        ini_set("memory_limit", "512M");
        $db->sql_to_xml_excel($arrExcel, 'relatorioListaObjetosObras', $cabecalho, '');
        return true;
    }

    public function listaObrasExcel($sql, $cabecalho)
    {
        global $db;

        $cabecalhoAlt = array_slice($cabecalho, 6);
        $cabecalho = array();
        foreach ($cabecalhoAlt as $cab) {
            array_push($cabecalho, strip_tags($cab));
        }

        // aplicando id como primeira coluna
        $arrayAlt = array();
        array_push($arrayAlt, 'ID');
        foreach ($cabecalho as $casa)
            if ($casa != 'ID')
                array_push($arrayAlt, $casa);
        $cabecalho = $arrayAlt;
        // --

        $arObras = $this->carregar($sql, null, 3600);
        $arrExcel = array();
        if (is_array($arObras)) {
            foreach ($arObras as $key => $v) {

                // campos novos
                // anexo

                // restrição - 1.atrasada(sim) 2.superada(não)
                $restricao = array();
                $restricao['superadas'] = "Não";
                if (strpos($v['r'], 'Superadas') !== false)
                    $restricao['superadas'] = "Sim";
                $restricao['pendentes'] = "Não";
                if (strpos($v['r'], 'Pendentes') !== false)
                    $restricao['pendentes'] = "Sim";
                $restricao['atraso'] = "Não";
                if (strpos($v['r'], 'Atraso') !== false)
                    $restricao['atraso'] = "Sim";
                // impedimento - 1.atrasada(sim) 2.superada(não)
                $impedimento = array();
                $impedimento['superadas'] = "Não";
                if (strpos($v['i'], 'Superadas') !== false)
                    $impedimento['superadas'] = "Sim";
                $impedimento['pendentes'] = "Não";
                if (strpos($v['i'], 'Pendentes') !== false)
                    $impedimento['pendentes'] = "Sim";
                $impedimento['atraso'] = "Não";
                if (strpos($v['i'], 'Atraso') !== false)
                    $impedimento['atraso'] = "Sim";

                $arrExcel[$key] = array('id' => $v['id'],
                    'restricoes_superadas' => $restricao['superadas'],
                    'restricoes_pendentes' => $restricao['pendentes'],
                    'restricoes_atrasadas' => $restricao['atraso'],
                    'impedimentos_superados' => $impedimento['superadas'],
                    'impedimentos_pendentes' => $impedimento['pendentes'],
                    'impedimentos_atrasados' => $impedimento['atraso'],
                    'preid' => $v['preid'],
                    'numero_processo' => $v['numero_processo'],
                    'numero_convenio' => $v['numero_convenio'],
                    'ano_convenio' => $v['ano_convenio'],
                    'descricao' => strip_tags($v['descricao']),
                    'ocrdtordemservico' => strip_tags($v['ocrdtordemservico']),
                    'unidade_implantadora' => $v['unidade_implantadora'],
                    'mesdsc' => $v['mesdsc'],
                    'municipio' => $v['municipio'],
                    'uf' => $v['uf'],
                    'inicio' => $v['inicio'],
                    'termino' => $v['termino'],
                    'situacao' => $v['situacao'],
                    'tipoparalisacao' => $v['tpldsc'],
                    'obrdtultvistoria' => $v['obrdtultvistoria'],
                    'qtddias' => $v['qtddias'],
                    'obrpercentultvistoria' => $v['obrpercentultvistoria'],
                    'empdtultvistoriaempresa' => $v['empdtultvistoriaempresa'],
                    'emppercentultvistoriaempresa' => $v['emppercentultvistoriaempresa'],
                    'tpodsc' => $v['tpodsc'],
                    'programa' => $v['programa'],
                    'fonte' => $v['fonte'],
                    'esfera' => $v['esfera'],
                    'ocrvalorexecucao' => str_replace('.', ',', (string) $v['ocrvalorexecucao']),
                    'numerodocumento' => $v['dopnumerodocumento'],
                    'datainclusao' => $v['dopdatainclusao'],
                    'obrvalorprevisto' => $v['obrvalorprevisto'],
                );
            }
        }
        ini_set("memory_limit", "1024M");
        $db->sql_to_xml_excel($arrExcel, 'relatorioListaObjetosObras', $cabecalho, '');

        return true;
    }

    public function cabecalhoListaSql($xls = null)
    {

        if ($xls) {
            $arCabecalho = array("Ação", "A", "F", "PG", "R", "I", 'Restrições Superadas', 'Restrições Pendentes', 'Restrições Atrasadas', 'Impedimentos Superados', 'Impedimentos Pendentes', 'Impedimentos Atrasados', "ID", "ID <br>Pré-Obra", "Nº Processo", "Nº Convênio", 'Ano Convênio', "Obra", "Data da Ordem de Serviço",
                "Unidade Implantadora", "Mesoregião", "Município", "UF", "Data de Início<br>da Execução", "Data de Término<br>da Execução", "Situação da<br>Obra", "Tipo de Paralisação",
                "Última Vistoria Instituição", "Qtd Dias Última Vistoria",
                "% Executado Instituição", "Última Vistoria Empresa", "% Executado Empresa", "Tipologia", "Programa", "Fonte", "Esfera", "Valor Contrato", "Nº Termo Compromisso", "Ano Termo Compromisso","Valor Previsto");
        } else {
            $arCabecalho = array("Ação", "R", "I", "V", "ID", "ID <br>Pré-Obra",  "Nº Processo", "Nº Convênio", 'Ano Convênio', "Obra",
                "Unidade Implantadora", "Município", "UF", "Data de Início<br>da Execução", "Data de Término<br>da Execução", "Situação da<br>Obra", "Última<br/> Vistoria<br/>Instituição",
                "%<br/>Executado<br/>Instituição", "Última<br/>Vistoria<br/>Empresa", "%<br/>Executado<br/>Empresa", "Tipologia", "Valor Contrato");
        }
        return $arCabecalho;
    }

    private function filtroSql(Array $param = array(), $tipoSelecao = null)
    {

        $where = Array();

        $param['orgid'] = (isset($param['orgid']) ? $param['orgid'] : $_SESSION['obras2']['orgid']);

        switch ($param['obrvinculada']) {
            case 'S':
                $where[] = " o.obrstatus = 'P' ";
                break;
            case 'N':
                $where[] = " o.obrstatus = 'A' ";
                break;
            case 'T':
                $where[] = " o.obrstatus IN ('A', 'P') ";
                break;
            default:
                $where[] = " o.obrstatus = 'A' ";
                break;
        }

        // Convenio
        if ($param['convenio']) {
//            $where[] = " Replace(Replace(Replace( TRIM(o.obrnumprocessoconv),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['convenio']}','.',''),'/',''),'-','') ";
            $where[] = " o.numconvenio = '{$param['convenio']}' ";
        }
        // Ano convenio
        if ($param['ano_convenio']) {
            $where[] = " o.obranoconvenio = '{$param['ano_convenio']}' ";
        }

        // processo
        if ($param['processo']) {
            $where[] = "
                (
                    Replace(Replace(Replace( TRIM(po1.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')
                    OR
                    Replace(Replace(Replace( TRIM(po2.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')
                )
            ";
        }
        // Ano processo
        if ($param['ano_processo']) {
            $where[] = "
                (
                substring(Replace(Replace(Replace( po1.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$param['ano_processo']}' OR
                substring(Replace(Replace(Replace( po2.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$param['ano_processo']}'
                )
             ";
        }

        // obridpai is null apenas para obras do historico - isso nao é verdade (por Adonias)
        // if ( $param['not(obridpai)'] && $param['obrvinculada'] == 'P'){
        // obridpai is null para a obra atual
        if ($param['not(obridpai)']) {
            $where[] = "o.obridpai IS NULL";
        }

        if ($param['frpid']) {
            $param['frpid'] = (array) $param['frpid'];
            $where[] = "o.frpid IN(" . implode(", ", $param['frpid']) . ")";
        }

        if ($param['tooid']) {
            $param['tooid'] = (array) $param['tooid'];
            $where[] = "o.tooid IN(" . implode(", ", $param['tooid']) . ")";
        }

        if ($param['esdid']) {
            $param['esdid'] = (array) $param['esdid'];
            $where[] = "d.esdid IN(" . implode(", ", $param['esdid']) . ")";
        }

        if ($param['entid']) {
            $param['entid'] = (array) $param['entid'];
            $where[] = "o.entid IN(" . implode(", ", $param['entid']) . ")";
        }

        if ($param['empid']) {
            $param['empid'] = (array) $param['empid'];
            $where[] = "o.empid IN(" . implode(", ", $param['empid']) . ")";
        }

        if ($param['orgid']) {
            $param['orgid'] = (array) $param['orgid'];
            $where[] = "e.orgid IN(" . implode(", ", $param['orgid']) . ")";
        }

        /*if ($param['obrbuscatexto']) {
            $where[] = " ( UPPER(o.obrnome) ILIKE UPPER('%" . $param['obrbuscatexto'] . "%') OR
			o.obrid::CHARACTER VARYING ILIKE UPPER('%" . $param['obrbuscatexto'] . "%') ) ";
        }*/

        if ($param['obrbuscatexto']) {
            $obrbuscatextoTemp = removeAcentos(str_replace("-"," ",(trim($param['obrbuscatexto']) )));

            if(!strpos ($obrbuscatextoTemp, ',')){
                $where[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $param['obrbuscatexto'] . "%') ) ";
            } else {
                $campos = explode(',', $obrbuscatextoTemp);
                $w = array();
                foreach ($campos as $c){
                    $c = trim($c);
                    $w[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') ) ";
                }

                $w = '(' . implode('OR', $w) . ')';
                $where[] = $w;
            }

        }

        if ($param['tobid']) {
            $param['tobid'] = (array) $param['tobid'];
            $where[] = "o.tobid IN(" . implode(", ", $param['tobid']) . ")";
        }

        if ($param['cloid']) {
            $param['cloid'] = (array) $param['cloid'];
            $where[] = "o.cloid IN(" . implode(", ", $param['cloid']) . ")";
        }

        if ($param['tpoid']) {
            $param['tpoid'] = (array) $param['tpoid'];
            $where[] = "o.tpoid IN(" . implode(", ", $param['tpoid']) . ")";
        }


        if(!empty($param['obrami'])){
            if($param['obrami'] == 'S')
                $param['IS(mi)'] = true;
            else
                $param['NOT(mi)'] = true;
        }


        if (!empty($param['IS(mi)'])) {
            $where[] = "o.tpoid IN (" . TPOID_MI_TIPO_B . ", " . TPOID_MI_TIPO_C . ")";
        }

        if (!empty($param['NOT(mi)'])) {
            $where[] = "o.tpoid NOT IN (" . TPOID_MI_TIPO_B . ", " . TPOID_MI_TIPO_C . ")";
        }

        if (!empty($param['emiid'])) {
            $param['emiid'] = (array) $param['emiid'];
            $where[] = "emi.emiid IN(" . implode(", ", $param['emiid']) . ")";
        }

        if ($param['prfid']) {
            $param['prfid'] = (array) $param['prfid'];
            $where[] = "e.prfid IN(" . implode(", ", $param['prfid']) . ")";
        }

        if ($param['moeid']) {
            $param['moeid'] = (array) $param['moeid'];
            $where[] = "e.moeid IN(" . implode(", ", $param['moeid']) . ")";
        }

        if ($param['estuf']) {
            $param['estuf'] = (array) $param['estuf'];
            $where[] = " mun.estuf IN('" . implode("', '", $param['estuf']) . "')";
        }

        if ($param['muncod']) {
            $param['muncod'] = (array) $param['muncod'];
            if(count($param['muncod']) > 0){
                $where[] = " mun.muncod IN('" . implode("', '", $param['muncod']) . "')";
            }
        }

        if ($param['empesfera']) {
            $param['empesfera'] = (array) $param['empesfera'];
            $where[] = "e.empesfera IN('" . implode("', '", $param['empesfera']) . "')";
        }

        if ($param['obrvalorprevisto_menor']) {
            $where[] = "o.obrvalorprevisto < " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_menor']);
        }

        if ($param['obrvalorprevisto_maior']) {
            $where[] = "o.obrvalorprevisto > " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_maior']);
        }

        if ($param['totalfoto'] == 'S') {
            $where[] = "obrsnfotos = true";
        } elseif ($param['totalfoto'] == 'N') {
            $where[] = "obrsnfotos = false OR obrsnfotos IS NULL";
        }

        if ($param['osexiste'] == 'S') {
            $where[] = "osm1.osmid IS NOT NULL or osm2.osmid IS NOT NULL or osm3.osmid IS NOT NULL ";
        } elseif ($param['osexiste'] == 'N') {
            $where[] = "osm1.osmid IS NULL AND osm2.osmid IS NULL AND osm3.osmid IS NULL";
        }

        if ($param['possui_vistoria'] == 'S') {
            $where[] = "obrdtultvistoria IS NOT NULL";
        } elseif ($param['possui_vistoria'] == 'N') {
            $where[] = "obrdtultvistoria IS NULL";
        }

        if ($param['moeempenhadoflag'] == 'S') {
            $where[] = "o.obrid IN (SELECT obrid FROM obras2.mobiliarioempenhado
                                          WHERE moeid IN (
                                                          SELECT MAX(moeid)
                                                          FROM obras2.mobiliarioempenhado
                                                          GROUP BY obrid) AND moeempenhadoflag = 'S')";
        } elseif ($param['moeempenhadoflag'] == 'N') {
            $where[] = "o.obrid NOT IN (SELECT obrid FROM obras2.mobiliarioempenhado
                                          WHERE moeid IN (
                                                          SELECT MAX(moeid)
                                                          FROM obras2.mobiliarioempenhado
                                                          GROUP BY obrid) AND moeempenhadoflag = 'S')";
        }

        if ($param['medidasexcecao'] == 'S') {
            $where[] = " medidasexcecao = TRUE ";
        } elseif ($param['medidasexcecao'] == 'N') {
            $where[] = " medidasexcecao = FALSE ";
        }

        // Repasse
        if ($param['repasse'] == 'S') {
            $where[] = "o.obrid IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')";
        } elseif ($param['repasse'] == 'N') {
            $where[] = "o.obrid NOT IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')";
        }

        if ($param['res_estado'] == 'S') {
            $where[] = "res_estado IS NOT NULL";
        } elseif ($param['res_estado'] == 'N') {
            $where[] = "res_estado IS NULL";
        }

        if ($param['res_superada'] == 'S') {
            $where[] = "res_estado = 1";
        } elseif ($param['res_superada'] == 'N') {
            $where[] = "res_estado > 1";
        }

        if ($param['ocraditivado'] == 'S') {
            $where[] = "ocraditivado = 't'";
        } elseif ($param['ocraditivado'] == 'N') {
            $where[] = " (ocraditivado IS NULL OR ocraditivado='f') ";
        }

        if ($param['responsavel'] == 'S') {
            $where[] = "e.empdtultvistoriaempresa IS NOT NULL";
        } elseif ($param['responsavel'] == 'N') {
            $where[] = "e.empdtultvistoriaempresa IS NULL";
        }
        
        if ($param['evomi'] == 'S') {
            $where[] = "evmi.emidtinclusao IS NOT NULL";
        } elseif ($param['evomi'] == 'N') {
            $where[] = "evmi.emidtinclusao IS NULL";
        }


        $ck_sql = "SELECT
                        ckf.obrid
                    FROM obras2.checklistfnde                    ckf
                    INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                    INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                    WHERE ckf.ckfstatus = 'A' AND que.queid = %s";

        //Filtros Checklist
        if ($param['chk_adm'] == 'S') {
            $where[] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM).")";
        } elseif ($param['chk_adm'] == 'N') {
            $where[] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM).")";
        }

        if ($param['chk_2pc'] == 'S') {
            $where[] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_2P).")";
        } elseif ($param['chk_2pc'] == 'N') {
            $where[] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_2P).")";
        }

        if ($param['chk_tec'] == 'S') {
            $where[] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC).")";
        } elseif ($param['chk_tec'] == 'N') {
            $where[] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC).")";
        }

        if ($param['chk_vinc'] == 'S') {
            $where[] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC).")";
        } elseif ($param['chk_vinc'] == 'N') {
            $where[] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC).")";
        }

        if ($param['fiscal_ativo'] == 'S') {
            $where[] = "e.empid IN (select
                                ur.empid
                            from obras2.usuarioresponsabilidade ur
                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                            where
                            ur.rpustatus = 'A' AND
                            u.suscod = 'A' AND
                            us.suscod = 'A' AND ur.empid IS NOT NULL
                            GROUP BY ur.empid
                            HAVING COUNT(*) > 1)";
        } elseif ($param['fiscal_ativo'] == 'N') {
            $where[] = " e.empid NOT IN (select
                                ur.empid
                            from obras2.usuarioresponsabilidade ur
                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                            where
                            ur.rpustatus = 'A' AND
                            u.suscod = 'A' AND
                            us.suscod = 'A' AND ur.empid IS NOT NULL
                            GROUP BY ur.empid
                            HAVING COUNT(*) > 1)";
        }


        if ($param['rsuid']) {
            $where[] = "rsuid = " . $param['rsuid'];
        }

        if ($param['stiid']) {
            $where[] = "stiid = " . $param['stiid'];
        }

        if ($param['strid']) {
            $where[] = "o.strid = " . $param['strid'];
        }

        if ($param['obrid']) {
            $where[] = "o.obrid = " . $param['obrid'];
//            $where[] = " ( o.obrid = " . $param['obrid']." OR o.obrid::CHARACTER VARYING ILIKE UPPER('%".$param['obrid']."%') ) ";
        }

        /**
         * Esse parametro vem da tela de relatorio FNDE
         */
        if ($param['dataparam']) {

            switch ($param['dataparam']) {
                case 'ate2010':
                    $where[] = " DATE_PART('year', o.obrdtvistoria) <= 2010 ";
                    break;
                case 'apos2010':
                    $where[] = " DATE_PART('year', o.obrdtvistoria) > 2010 ";
                    break;
                case 'semconclusao':
                    $where[] = " o.obrdtvistoria is null ";
                    break;
            }
        }

        switch ($param['nivelpreenchimento']) {
            /* 1 - Verde    -> x < 45              */
            /* 2 - Amarelo  -> 45 > x < 60         */
            /* 3 - Vermelho -> x > 60              */
            case 1 : //Verde
                $where[] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) < 45 ) 
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true 
                                ELSE
                                    ( DATE_PART('days', NOW() - sup.supdata  ) < 45 ) 
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true 
                                END  ";
                break;
            case 2 :
                $where[] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN 
                                    ( (DATE_PART('days', NOW() - o.obrdtultvistoria ) >= 45) AND DATE_PART('days', NOW() - o.obrdtultvistoria ) < 60 ) 
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( (DATE_PART('days', NOW() - sup.supdata  ) >= 45) AND DATE_PART('days', NOW() - sup.supdata ) < 60  ) 
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true 
                                END";
                break;
            case 3 :
                $where[] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN 
                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) >= 60 ) 
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true 
                                ELSE
                                    ( DATE_PART('days', NOW() - sup.supdata  ) >= 60 ) 
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true 
                                END  ";
                break;
        }
        
        if ($param['percentualinicial'] != '') {
            $where[] = "COALESCE(obrpercentultvistoria, 0) >= " . $param['percentualinicial'];
        }

        if ($param['percentualfinal'] != '') {
            if ($param['percentualfinal'] < 100) {
                $where[] = "COALESCE(obrpercentultvistoria, 0) <= " . $param['percentualfinal'];
            }
        }

        switch ($tipoSelecao) {
            case 'vinculoContrato':
                $where[] = "o.obrid NOT IN (SELECT
                                                    obrid
                                            FROM
                                                    obras2.obrascontrato oc
                                            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                                            WHERE
                                                    " . ( $param['crtid'] ? " c.crtid != {$param['crtid']} AND " : "" ) . "
                                                    oc.ocrstatus = 'A' )";

                if ($param['licid']) {
                    $where[] = "o.obrid IN (SELECT
                                                    obrid
                                            FROM
                                                    obras2.obralicitacao ol
                                            JOIN obras2.licitacao l ON l.licid = ol.licid AND l.licstatus = 'A'
                                            WHERE
                                                    ol.licid = {$param['licid']} AND
                                                    ol.oblstatus = 'A' )";
                }

                break;
        }

        if ($param['ultatualizacao']) {
            switch ($param['ultatualizacao']) {
                case 1:
                    $where[] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) < 45";
                    break;
                case 2:
                    $where[] = "( DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) >= 45 AND
                                  DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) <= 60 )";
                    break;
                case 3:
                    $where[] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) > 60";
                    break;
            }
        }

        if ($param['funcionamentoflag']) {
            switch ($param['funcionamentoflag']) {
                case 1: // em funcionamento
                    $queryComplement = 'AND moedtiniciofunc < NOW()';
                    break;
                case 2: // previsão de inauguração
                    $queryComplement = 'AND moedtprevinauguracao BETWEEN \''.ajusta_data($param['moedtprevinauguracao_i']).'\' AND \''.ajusta_data($param['moedtprevinauguracao_f']).'\'';
                    break;
                case 3: //previsão inicio de funcionamento
                    $queryComplement = 'AND moedtpreviniciofunc BETWEEN \''.ajusta_data($param['moedtpreviniciofunc_i']).'\' AND \''.ajusta_data($param['moedtpreviniciofunc_f']).'\'';
                    break;
            }

            $where[] = sprintf("CASE WHEN (SELECT
                            CASE WHEN moeid IS NOT NULL THEN TRUE ELSE FALSE END
                            FROM obras2.mobiliarioempenhado
                            WHERE obrid = o.obrid %s
                            ORDER BY moeid DESC
                            LIMIT 1) THEN true
                         ELSE false END", $queryComplement);
        }



        if ($param['arqidcontrato'] == 'S') {
            $where[] = "c.arqidcontrato IS NOT NULL";
        } elseif ($param['arqidcontrato'] == 'N') {
            $where[] = "c.arqidcontrato IS NULL";
        }

        return $where;
    }

    private function joinSql(Array $param = array())
    {

        $join = array();

        if ($param['excel']) {
            $join['mesoregiao'] = "LEFT JOIN territorios.mesoregiao meso ON meso.mescod = mun.mescod";
            $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";
        }

        if($param['nivelpreenchimento'] || $param['rsuid']){
            $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";
        }

//        if ($param['estuf']) {
            $join['endereco']  = " LEFT JOIN entidade.endereco     ede ON ede.endid  = o.endid AND ede.endstatus = 'A' ";
//        }
//
//        if ($param['muncod'] && empty($param['estuf'])) {
//            $join['endereco']  = " LEFT JOIN entidade.endereco     ede ON ede.endid  = o.endid AND ede.endstatus = 'A' 
//                                   LEFT JOIN territorios.municipio mun ON mun.muncod = ede.muncod ";
//        }

        if (!possui_perfil(Array(PFLCOD_SUPER_USUARIO))) {
            //			$arrEst = Array(PFLCOD_CADASTRADOR_INSTITUCIONAL,
            //							PFLCOD_CONSULTA_ESTADUAL,
            //							PFLCOD_EMPRESA_CONTRATADA);
            //			$arrObr = Array(PFLCOD_EMPRESA_CONTRATADA,
            //							PFLCOD_SUPERVISOR_UNIDADE);
            //			$arrOrg = Array(PFLCOD_ADMINISTRADOR,
            //							PFLCOD_CADASTRADOR_INSTITUCIONAL,
            //							PFLCOD_CONSULTA_ESTADUAL,
            //							PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            //							PFLCOD_SUPERVISOR_MEC);
            //			$arrUni = Array(PFLCOD_AUDITOR_INTERNO,
            //							PFLCOD_CADASTRADOR_INSTITUCIONAL,
            //							PFLCOD_CONSULTA_UNIDADE,
            //							PFLCOD_GESTOR_UNIDADE,
            //							PFLCOD_SUPERVISOR_UNIDADE);

            $arrObr = Array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                PFLCOD_EMPRESA_VISTORIADORA_GESTOR/* ,
                      PFLCOD_SUPERVISOR_UNIDADE */);

            $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

            $arrOrg = Array(PFLCOD_ADMINISTRADOR,
                PFLCOD_CADASTRADOR_INSTITUCIONAL,
                //							PFLCOD_CONSULTA_ESTADUAL,
                PFLCOD_CONSULTA_TIPO_DE_ENSINO,
                PFLCOD_SUPERVISOR_MEC,
                PFLCOD_GESTOR_MEC);

            $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

            $resp = array();
            $arPflcod = array();

            if (possui_perfil($arrEst)) {
//                if(!$_POST['abreBuscaAvancada']){ // Para o perfil PFLCOD_CONSULTA_ESTADUAL trazer somente as obras do seu estado, mas caso ele não faça uma busca avançada
                    $resp[] = "urs.estuf = ende.estuf ";
                    $arPflcod = array_merge($arPflcod, $arrEst);
//                }
            }

            if (possui_perfil($arrObr)) {
                $resp[] = "urs.empid = e.empid ";
                $arPflcod = array_merge($arPflcod, $arrObr);
            }

            if (possui_perfil($arrOrg)) {
                $resp[] = "urs.orgid = e.orgid ";
                $arPflcod = array_merge($arPflcod, $arrOrg);
            }

            if (possui_perfil($arrUni)) {
                $resp[] = "urs.entid = e.entidunidade ";
                $arPflcod = array_merge($arPflcod, $arrUni);
            }

            if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid = ($arEmpid ? $arEmpid : array(0));

                $resp[] = " urs.entid = e.entidunidade AND e.empid IN('" . implode("', '", $arEmpid) . "')";
                $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
                $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;
            }

            if (count($resp)) {
                $arPflcod = array_unique($arPflcod);
                $join['usuarioresponsabilidade'] = "JOIN obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    (" . implode(' OR ', $resp) . ")";
            }
        }

        return $join;
    }

    private function colunaAcaoMi($tipo)
    {

        $acaoAlterar = '';
        $acaoOS      = '';

        if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE))) {
            $acaoAlterar = "<img align=\"absmiddle\"
                                 src=\"/imagens/alterar.gif\"
                                 style=\"cursor: pointer\"
                                 onclick=\"javascript: alterarObr(\'' || o.obrid || '\');\"
                                 title=\"Alterar Obra\" />";
        }

        if (possui_perfil(array( PFLCOD_SUPER_USUARIO,
                                 PFLCOD_GESTOR_MEC,
                                 PFLCOD_SUPERVISOR_UNIDADE,
                                 PFLCOD_GESTOR_UNIDADE,
                                 PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                                 PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                                 PFLCOD_EMPRESA_CONTRATADA))) {

            $acaoOS = '<img align="absmiddle"
                            src="/imagens/aceite_os_2.png"
                            style="cursor: pointer"
                            onclick="javascript: visualizarOsObra(\'|| o.obrid ||\');"
                            title="Visualizar as OS">';
        }

        switch ($tipo) {
            case 'aguardandoEmissao':
                $acao = "
                                    '<center>
                                            <div style=\"width:80px\">
                                                <img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/principal.gif\"
                                                    style=\"cursor: pointer\"
                                                    onclick=\"javascript: cadastrarOS(\'' || o.obrid || '\', 2);\"
                                                    title=\"Cadastro da OS de Sondagem\">
                                                <img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/principal.gif\"
                                                    style=\"cursor: pointer\"
                                                    onclick=\"javascript: cadastrarOS(\'' || o.obrid || '\', 3);\"
                                                    title=\"Cadastro da OS do Projeto de Implantação\">
                                                <img
                                                    align=\"absmiddle\"
                                                    src=\"/imagens/principal.gif\"
                                                    style=\"cursor: pointer\"
                                                    onclick=\"javascript: cadastrarOS(\'' || o.obrid || '\', 1);\"
                                                    title=\"Cadastro da OS de Execução\">
                                                {$acaoAlterar}
                                            </div>
                                      </center>'
                                ";
                break;
            case 'aguardandoAceite':


                $acao = "

                                                '<center>
							<div style=\"width:80px\">
                                                        '||
                                                        CASE WHEN osm2.tomid = 2 THEN
                                                            '<img
                                                                align=\"absmiddle\"
                                                                src=\"/imagens/aceite_os_2.png\"
                                                                style=\"cursor: pointer\"
                                                                onclick=\"javascript: avaliarOS(\'' || o.obrid || '\', \'' || osm2.tomid || '\');\"
                                                                title=\"Aceite da OS de Sondagem\">'
                                                        ELSE
                                                            '<img
                                                                align=\"absmiddle\"
                                                                src=\"/imagens/aceite_os_2_inativo.png\"
                                                                title=\"Não existe OS de Sondagem\">'
                                                        END
                                                        ||
                                                        CASE WHEN osm3.tomid = 3 THEN
                                                            '<img
                                                                align=\"absmiddle\"
                                                                src=\"/imagens/aceite_os_2.png\"
                                                                style=\"cursor: pointer\"
                                                                onclick=\"javascript: avaliarOS(\'' || o.obrid || '\', \'' || osm3.tomid || '\');\"
                                                                title=\"Aceite da OS do Projeto de Implantação\">'
                                                        ELSE
                                                            '<img
                                                                align=\"absmiddle\"
                                                                src=\"/imagens/aceite_os_2_inativo.png\"
                                                                title=\"Não existe OS do Projeto de Implantação\">'
                                                        END
                                                        ||
                                                        CASE WHEN osm1.tomid = 1 THEN
                                                            '<img
                                                                    align=\"absmiddle\"
                                                                    src=\"/imagens/aceite_os_2.png\"
                                                                    style=\"cursor: pointer\"
                                                                    onclick=\"javascript: avaliarOS(\'' || o.obrid || '\', \'' || osm1.tomid || '\');\"
                                                                    title=\"Aceite da OS de Execução\">'
                                                        ELSE
                                                            '<img
                                                                align=\"absmiddle\"
                                                                src=\"/imagens/aceite_os_2_inativo.png\"
                                                                title=\"Não existe OS de Execução\">'
                                                        END
                                                        ||
                                                        '
                                                        {$acaoAlterar}
							</div>
						  </center>'
                                    ";

                break;
            default:
                $acao = "'<center><div style=\"width:80px\">
							'|| CASE WHEN o.preid IS NOT NULL THEN '<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/icone_capacete_104.png\"
			 					style=\"cursor: pointer\"
			 					onclick=\"javascript: abreAnaliseTerrenoObjeto(\'' || o.obrid || '\',\'' || o.preid || '\');\"
			 					title=\"Análise do Terreno\">' ELSE '' END ||'
							<img
			 					align=\"absmiddle\"
			 					src=\"/imagens/edit_on.gif\"
			 					style=\"cursor: pointer\"
			 					onclick=\"javascript: supervisaoObrMi(\'' || o.obrid || '\');\"
			 					title=\"Evolução MI\">
                                                                {$acaoAlterar}
                                                                {$acaoOS}
						  </div></center>'";
//                $acao = "'<center><div style=\"width:80px\">
//							'|| CASE WHEN o.preid IS NOT NULL THEN '<img
//			 					align=\"absmiddle\"
//			 					src=\"/imagens/icone_capacete_104.png\"
//			 					style=\"cursor: pointer\"
//			 					onclick=\"javascript: abreAnaliseTerrenoObjeto(\'' || o.obrid || '\',\'' || o.preid || '\');\"
//			 					title=\"Análise do Terreno\">' ELSE '' END ||'
//							<img
//			 					align=\"absmiddle\"
//			 					src=\"/imagens/edit_on.gif\"
//			 					style=\"cursor: pointer\"
//			 					onclick=\"javascript: supervisaoObrMi(\'' || o.obrid || '\');\"
//			 					title=\"Medição da obra\"> --- Linha alterada
//                                                                {$acaoAlterar}
//                                                                {$acaoOS}
//						  </div></center>'";
        }

        return $acao;
    }

    private function selectSqlMi($tipo)
    {

        $acao = $this->colunaAcaoMi($tipo);

        $R = '\'<img title="Restrições \'|| CASE WHEN res_estado = 1 THEN \'Superadas\' WHEN res_estado = 2 THEN \'Pendentes\' ELSE \'em Atraso\' END || \'" style="cursor:pointer; width:15px;"
					 onclick="window.location.href = \'\'obras2.php?modulo=principal/listaRestricao&acao=O&obrid=\'|| o.obrid ||\'\'\';"
					 src="/imagens/obras/atencao\'|| CASE WHEN res_estado = 1 THEN \'_verde\' WHEN res_estado = 2 THEN \'\' ELSE \'_vermelho\' END || \'.png">\'';
        $I = '\'<img title="Impedimentos \'|| CASE WHEN inc_estado = 1 THEN \'Superadas\' WHEN inc_estado = 2 THEN \'Pendentes\' ELSE \'em Atraso\' END || \'" style="cursor:pointer; width:15px;"
					 onclick="window.location.href = \'\'obras2.php?modulo=principal/listaRestricao&acao=O&obrid=\'|| o.obrid ||\'\'\';"
					 src="/imagens/obras/atencao\'|| CASE WHEN inc_estado = 1 THEN \'_verde\' WHEN inc_estado = 2 THEN \'\' ELSE \'_vermelho\' END || \'.png">\'';

        $const = get_defined_constants();
        switch ($tipo) {

            case 'aguardandoEmissao':

                $imgOs = "
                             CASE WHEN osm2.tomid = 2 AND osmd2.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem cadastrada para essa obra\">'
				 			 WHEN osm2.tomid = 2 AND (osmd2.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem aguardando aceite para essa obra\">'
                             WHEN osm2.tomid = 2 AND (osmd2.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem concluída para essa obra\">'
                             WHEN osm2.tomid = 2 AND osmd2.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Sondagem recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Sondagem cadastrada para essa obra\">'
                             END
                             ||
                             CASE WHEN osm3.tomid = 3 AND osmd3.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação cadastrada para essa obra\">'
				 			 WHEN osm3.tomid = 3 AND (osmd3.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação aguaradando aceite para essa obra\">'
                             WHEN osm3.tomid = 3 AND (osmd3.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação concluída para essa obra\">'
                             WHEN osm3.tomid = 3 AND osmd3.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Implantação recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Implantação cadastrada para essa obra\">'
                             END
						     ||
						     CASE WHEN osm1.tomid = 1 AND osmd1.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução cadastrada para essa obra\">'
				 			 WHEN osm1.tomid = 1 AND (osmd1.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução aguardando aceite para essa obra\">'
                             WHEN osm1.tomid = 1 AND (osmd1.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução concluida para essa obra\">'
                             WHEN osm1.tomid = 1 AND osmd1.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Execução recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Execução cadastrada para essa obra\">'
                             END

				";

                $select = <<<EOT
							{$acao} AS acao,
							{$imgOs} AS os,
							CASE WHEN res_estado IS NOT NULL THEN $R ELSE '' END as r,
							CASE WHEN inc_estado IS NOT NULL THEN $I ELSE '' END as i,
							o.obrid as id,
							o.preid as preid,
							'<a href="javascript: alterarObr(\'' || o.obrid || '\');">(' || o.obrid || ') ' || obrnome || '</a>' as descricao,
							'( ' || TO_CHAR(emi.emicnpj::int8, '00"."000"."000"/"0000"-"00') || ' ) ' || emi.emidsc AS emidsc,
							entnome as unidade_implantadora,
							mun.mundescricao||' - '||mun.estuf as municipio,
                                                        ende.endbai as bairro,
							--TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
							--TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
							ed.esddsc as situacao,
							'<FONT ' ||
                                            CASE WHEN ed.esdid IN (693) THEN
                                                'COLOR="#3276B1">' ||
                                                CASE WHEN obrdtultvistoria IS NOT NULL THEN
                                                        to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
                                                     ELSE
                                                        to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )'
                                                END
                                            -- Se tiver ultima vistoria e a obra estiver paralisada ou execucao, aplica as cores
                                            WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN

                                                CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) < 45 THEN
                                                        'COLOR="#3CC03C" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                                                        'COLOR="#FAD200" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 60 THEN
                                                        'COLOR="#FF0000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'

                                            -- Se não tiver ultima vistoria e estiver em execução ou paralisada usa a data de inclusao, aplica as cores
                                            WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) THEN

                                                CASE WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) < 45 THEN
                                                        'COLOR="#3CC03C" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) >= 45 AND DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) < 60 THEN
                                                        'COLOR="#FAD200" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) >= 60 THEN
                                                        'COLOR="#FF0000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )'

                                            -- Se tiver vistoria e não esta em execucao ou paralisada, sem cores
                                            WHEN o.obrdtultvistoria IS NULL THEN
                                                CASE WHEN DATE_PART('days', NOW() - obrdtinclusao) <= 45 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 45 AND DATE_PART('days', NOW() - obrdtinclusao) <= 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'

                                            -- Se tiver vistoria e não esta em execucao ou paralisada, sem cores
                                            ELSE

                                                CASE WHEN DATE_PART('days', NOW() - obrdtinclusao) <= 45 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 45 AND DATE_PART('days', NOW() - obrdtinclusao) <= 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'

                                            END
                                        || '</FONT>' AS ultima_atualizacao,
							to_char( obrdtultvistoria,'DD/MM/YYYY') as dtvistoria,
							obrpercentultvistoria,
							tpo.tpodsc
EOT;
                break;
            case 'aguardandoAceite':
                $imgOs = "

                            CASE WHEN osm2.tomid = 2 AND osmd2.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem cadastrada para essa obra\">'
                            WHEN osm2.tomid = 2 AND (osmd2.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem aguardando aceite para essa obra\">'
                            WHEN osm2.tomid = 2 AND (osmd2.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem concluida para essa obra\">'
                            WHEN osm2.tomid = 2 AND osmd2.esdid = 910 THEN
                                                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Sondagem recusada.\">'
				 			ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Sondagem cadastrada para essa obra!\">'
                            END
                            ||
                            CASE WHEN osm3.tomid = 3 AND osmd3.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação cadastrada para essa obra\">'
				 		    WHEN osm3.tomid = 3 AND (osmd3.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS de Implantação aguardando aceite para essa obra\">'
                            WHEN osm3.tomid = 3 AND (osmd3.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS de Implantação concluida para essa obra\">'
                            WHEN osm3.tomid = 3 AND osmd3.esdid = 910 THEN
                                                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Implantação recusada.\">'
				 			ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS de Implantação cadastrada para essa obra\">'
                            END
                            ||
                            CASE WHEN osm1.tomid = 1 AND osmd1.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução cadastrada para essa obra\">'
				 		    WHEN osm1.tomid = 1 AND (osmd1.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução aguardando aceite para essa obra\">'
                            WHEN osm1.tomid = 1 AND (osmd1.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução concluida para essa obra\">'
                            WHEN osm1.tomid = 1 AND osmd1.esdid = 910 THEN
                                                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Execução recusada.\">'
				 			ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Execução cadastrada para essa obra\">'
                            END
                                                  ";

                $select = <<<EOT
							{$acao} AS acao,
							{$imgOs} AS os,
							CASE WHEN res_estado IS NOT NULL THEN $R ELSE '' END as r,
							CASE WHEN inc_estado IS NOT NULL THEN $I ELSE '' END as i,
							o.obrid as id,
							o.preid as preid,
							p_conv.pronumeroprocesso,
                            substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) anoprocesso,
                            p_conv.termo_convenio,
                            p_conv.ano_termo_convenio,
							'<a href="javascript: alterarObr(\'' || o.obrid || '\');">(' || o.obrid || ') ' || obrnome || '</a>' as descricao,
							'( ' || TO_CHAR(emi.emicnpj::int8, '00"."000"."000"/"0000"-"00') || ' ) ' || emi.emidsc AS emidsc,
							entnome as unidade_implantadora,
							mun.mundescricao||' - '||mun.estuf as municipio,
                                                        ende.endbai as bairro,
							TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
							TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
							ed.esddsc as situacao,
							'<FONT ' ||
                                            CASE WHEN ed.esdid IN (693) THEN
                                                'COLOR="#3276B1">' ||
                                                CASE WHEN obrdtultvistoria IS NOT NULL THEN
                                                        to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
                                                     ELSE
                                                        to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )'
                                                END
                                            -- Se tiver ultima vistoria e a obra estiver paralisada ou execucao, aplica as cores
                                            WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN

                                                CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) < 45 THEN
                                                        'COLOR="#3CC03C" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                                                        'COLOR="#FAD200" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 60 THEN
                                                        'COLOR="#FF0000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'

                                            -- Se não tiver ultima vistoria e estiver em execução ou paralisada usa a data de inclusao, aplica as cores
                                            WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) THEN

                                                CASE WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) < 45 THEN
                                                        'COLOR="#3CC03C" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) >= 45 AND DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) < 60 THEN
                                                        'COLOR="#FAD200" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) >= 60 THEN
                                                        'COLOR="#FF0000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )'

                                            -- Se tiver vistoria e não esta em execucao ou paralisada, sem cores
                                            WHEN o.obrdtultvistoria IS NULL THEN
                                                CASE WHEN DATE_PART('days', NOW() - obrdtinclusao) <= 45 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 45 AND DATE_PART('days', NOW() - obrdtinclusao) <= 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'

                                            -- Se tiver vistoria e não esta em execucao ou paralisada, sem cores
                                            ELSE

                                                CASE WHEN DATE_PART('days', NOW() - obrdtinclusao) <= 45 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 45 AND DATE_PART('days', NOW() - obrdtinclusao) <= 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'

                                            END
                                        || '</FONT>' AS ultima_atualizacao,
							to_char( obrdtultvistoria,'DD/MM/YYYY') as dtvistoria,
							obrpercentultvistoria,
							tpo.tpodsc,
							ocrvalorexecucao,
                                                        (   SELECT  sum(emipecentvalidadoedif) as soma
                                                            FROM obras2.evolucaomi
                                                            WHERE emistatus = 'A'
                                                              AND emivalidador IS NOT NULL
                                                              AND obrid = o.obrid
                                                        )as soma_evo
EOT;
//                                                                 ver(simec_htmlentities($os),d);
                break;
            default:

                $imgOs = "

                            CASE WHEN osm2.tomid = 2 AND osmd2.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem cadastrada para essa obra\">'
                            WHEN osm2.tomid = 2 AND (osmd2.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem aguardando aceite para essa obra\">'
                            WHEN osm2.tomid = 2 AND (osmd2.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem concluida para essa obra\">'
                            WHEN osm2.tomid = 2 AND osmd2.esdid = 910 THEN
                                                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Sondagem recusada.\">'
				 			ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Sondagem cadastrada para essa obra!\">'
                            END
                            ||
                            CASE WHEN osm3.tomid = 3 AND osmd3.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação cadastrada para essa obra\">'
				 		    WHEN osm3.tomid = 3 AND (osmd3.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS de Implantação aguardando aceite para essa obra\">'
                            WHEN osm3.tomid = 3 AND (osmd3.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS de Implantação concluida para essa obra\">'
                            WHEN osm3.tomid = 3 AND osmd3.esdid = 910 THEN
                                                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Implantação recusada.\">'
				 			ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS de Implantação cadastrada para essa obra\">'
                            END
                            ||
                            CASE WHEN osm1.tomid = 1 AND osmd1.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução cadastrada para essa obra\">'
				 		    WHEN osm1.tomid = 1 AND (osmd1.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução aguardando aceite para essa obra\">'
                            WHEN osm1.tomid = 1 AND (osmd1.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução concluida para essa obra\">'
                            WHEN osm1.tomid = 1 AND osmd1.esdid = 910 THEN
                                                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Execução recusada.\">'
				 			ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Execução cadastrada para essa obra\">'
                            END
                                                  ";
                $select = <<<EOT
                        {$acao} AS acao,
                        {$imgOs} AS imgos,
                        CASE WHEN res_estado IS NOT NULL THEN $R ELSE '' END as r,
                        CASE WHEN inc_estado IS NOT NULL THEN $I ELSE '' END as i,
                        o.obrid as id,
                        o.preid as preid,
                        p_conv.pronumeroprocesso,
                        substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) anoprocesso,
                        p_conv.termo_convenio,
                        p_conv.ano_termo_convenio,
                        '<a href="javascript: alterarObr(\'' || o.obrid || '\');">(' || o.obrid || ') ' || obrnome || '</a>' as descricao,
                        '( ' || TO_CHAR(emi.emicnpj::int8, '00"."000"."000"/"0000"-"00') || ' ) ' || emi.emidsc AS emidsc,
                        entnome as unidade_implantadora,
                        mun.mundescricao||' - '||mun.estuf as municipio,
                        ende.endbai as bairro,
                        TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
                        TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
                        ed.esddsc as situacao,
                        '<FONT ' ||
                                            CASE WHEN ed.esdid IN (693) THEN
                                                'COLOR="#3276B1">' ||
                                                CASE WHEN obrdtultvistoria IS NOT NULL THEN
                                                        to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
                                                     ELSE
                                                        to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )'
                                                END
                                            -- Se tiver ultima vistoria e a obra estiver paralisada ou execucao, aplica as cores
                                            WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN

                                                CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) < 45 THEN
                                                        'COLOR="#3CC03C" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                                                        'COLOR="#FAD200" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 60 THEN
                                                        'COLOR="#FF0000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'

                                            -- Se não tiver ultima vistoria e estiver em execução ou paralisada usa a data de inclusao, aplica as cores
                                            WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) THEN

                                                CASE WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) < 45 THEN
                                                        'COLOR="#3CC03C" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) >= 45 AND DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) < 60 THEN
                                                        'COLOR="#FAD200" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) >= 60 THEN
                                                        'COLOR="#FF0000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )'

                                            -- Se tiver vistoria e não esta em execucao ou paralisada, sem cores
                                            WHEN o.obrdtultvistoria IS NULL THEN
                                                CASE WHEN DATE_PART('days', NOW() - obrdtinclusao) <= 45 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 45 AND DATE_PART('days', NOW() - obrdtinclusao) <= 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'

                                            -- Se tiver vistoria e não esta em execucao ou paralisada, sem cores
                                            ELSE

                                                CASE WHEN DATE_PART('days', NOW() - obrdtinclusao) <= 45 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada em até 45 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 45 AND DATE_PART('days', NOW() - obrdtinclusao) <= 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada entre 45 e 60 dias">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 60 THEN
                                                        'COLOR="#000000" TITLE="Esta obra foi atualizada a mais de 60 dias">'
                                                END
                                                || to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'

                                            END
                                        || '</FONT>' AS ultima_atualizacao,
                        to_char( obrdtultvistoria,'DD/MM/YYYY') as dtvistoria,
                        obrpercentultvistoria,
                        tpo.tpodsc,
                        ocrvalorexecucao,
                        (   SELECT  sum(emipecentvalidadoedif) as soma
                            FROM obras2.evolucaomi
                            WHERE emistatus = 'A'
                              AND emivalidador IS NOT NULL
                              AND obrid = o.obrid
                        )as soma_evo

EOT;
        }
        
//        ver(simec_htmlentities($select));
        return $select;
    }

    private function joinSqlMi($tipo)
    {
        $joinMi = array();

        /*
         * Tratamento Perfil 'Empresa MI - Gestor'
         */
        if (possui_perfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL)) && !$this->testa_superuser()) {
            $joinEmpresaMI = " 	INNER JOIN obras2.empresami_uf            euf ON euf.estuf = ende.estuf AND euf.eufstatus = 'A'
                              --INNER JOIN obras2.usuarioresponsabilidade urs ON urs.emiid = euf.emiid  AND urs.rpustatus = 'A' AND urs.usucpf = '{$_SESSION['usucpf']}'
                                INNER JOIN obras2.empresami               emi ON emi.emiid = euf.emiid  AND emi.emistatus = 'A'";
        } else {
            $joinEmpresaMI = "LEFT JOIN obras2.empresami_uf euf ON euf.estuf = ende.estuf AND euf.eufstatus = 'A'
                              LEFT JOIN obras2.empresami    emi ON emi.emiid = euf.emiid AND emi.emistatus = 'A'";
        }

        switch ($tipo) {
            case 'aguardandoEmissao':
                $joinMi[] = "obras2.obras o
                                INNER JOIN entidade.endereco	  ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "

                                LEFT JOIN territorios.municipio    mun ON mun.muncod = ende.muncod
                                LEFT JOIN workflow.documento         d ON d.docid    = o.docid -- AND d.tpdid = " . TPDID_OBJETO . "
                                LEFT JOIN workflow.estadodocumento  ed ON ed.esdid   = d.esdid
                                LEFT JOIN entidade.entidade        ent ON ent.entid  = o.entid
                                LEFT JOIN obras2.tipologiaobra     tpo ON tpo.tpoid  = o.tpoid
                                LEFT JOIN obras2.empreendimento      e ON e.empid    = o.empid AND e.empstatus = 'A'
                                LEFT JOIN obras2.obrascontrato       oc ON oc.obrid   = o.obrid AND oc.ocrstatus = 'A'
                                LEFT JOIN obras2.contrato c             ON c.crtid    = oc.crtid AND c.crtstatus = 'A'

                                LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'

                                {$joinEmpresaMI}

                                -- Join com a ultima supervisão ativa para o tipo determinado
                                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 1 GROUP BY osmt.tomid, osmt.obrid ) AS osm1 ON osm1.obrid = o.obrid
                                LEFT JOIN workflow.documento osmd1 ON osm1.docid = osmd1.docid

                                -- Join com a ultima supervisão ativa para o tipo determinado
                                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 2 GROUP BY osmt.tomid, osmt.obrid ) AS osm2 ON osm2.obrid = o.obrid
                                LEFT JOIN workflow.documento osmd2 ON osm2.docid = osmd2.docid

                                -- Join com a ultima supervisão ativa para o tipo determinado
                                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 3 GROUP BY osmt.tomid, osmt.obrid ) AS osm3 ON osm3.obrid = o.obrid
                                LEFT JOIN workflow.documento osmd3 ON osm3.docid = osmd3.docid

                                LEFT JOIN (
                                            SELECT
                                                    max( res_estado ) as res_estado,
                                                    obrid
                                            FROM
                                             (
                                                SELECT DISTINCT
                                                        CASE
                                                                WHEN rstdtsuperacao IS NOT NULL THEN 1
                                                                WHEN rstdtprevisaoregularizacao > now() THEN 2
                                                                WHEN rstdtprevisaoregularizacao < now() THEN 3
                                                                ELSE 2
                                                        END as res_estado,
                                                        obrid
                                                FROM
                                                        obras2.restricao
                                                WHERE
                                                        obrid IS NOT NULL AND
                                                        rstitem = 'R' AND
                                                        rststatus = 'A'
                                             ) as  foo
                                            GROUP BY
                                                    obrid
                                          ) res ON res.obrid = o.obrid
                                LEFT JOIN (
                                            SELECT
                                                    max( inc_estado ) as inc_estado,
                                                    obrid
                                            FROM
                                                    (
                                                    SELECT DISTINCT
                                                            CASE
                                                                    WHEN rstdtsuperacao IS NOT NULL THEN 1
                                                                    WHEN rstdtprevisaoregularizacao > now() THEN 2
                                                                    WHEN rstdtprevisaoregularizacao < now() THEN 3
                                                                    ELSE 2
                                                            END as inc_estado,
                                                            obrid
                                                    FROM
                                                            obras2.restricao
                                                    WHERE
                                                            obrid IS NOT NULL AND
                                                            rstitem = 'I' AND
                                                            rststatus = 'A'
                                                    ) as  foo
                                            GROUP BY
                                                    obrid
                                            ) inc ON inc.obrid = o.obrid
                           ";
                break;
            case 'aguardandoAceite':
                $joinMi[] = " obras2.obras o
                                INNER JOIN entidade.endereco      ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
                                INNER JOIN obras2.ordemservicomi   osm ON osm.obrid  = o.obrid AND osm.osmstatus = 'A'
                                
                                LEFT JOIN territorios.municipio    mun ON mun.muncod = ende.muncod 
                                LEFT JOIN workflow.documento         d ON d.docid    = o.docid AND d.tpdid = " . TPDID_OBJETO . "
                                LEFT JOIN workflow.estadodocumento  ed ON ed.esdid   = d.esdid
                                LEFT JOIN entidade.entidade        ent ON ent.entid  = o.entid
                                LEFT JOIN obras2.tipologiaobra     tpo ON tpo.tpoid  = o.tpoid
                                LEFT JOIN obras2.empreendimento      e ON e.empid    = o.empid AND e.empstatus = 'A'
                                LEFT JOIN obras2.obrascontrato      oc ON oc.obrid   = o.obrid AND oc.ocrstatus = 'A'
                                LEFT JOIN obras2.contrato c             ON c.crtid    = oc.crtid AND c.crtstatus = 'A'

                                LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'

                                -- Join com a ultima supervisão ativa para o tipo determinado
                                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 1 GROUP BY osmt.tomid, osmt.obrid ) AS osm1 ON osm1.obrid = o.obrid
                                LEFT JOIN workflow.documento osmd1 ON osm1.docid = osmd1.docid
                                
                                -- Join com a ultima supervisão ativa para o tipo determinado
                                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 2 GROUP BY osmt.tomid, osmt.obrid ) AS osm2 ON osm2.obrid = o.obrid
                                LEFT JOIN workflow.documento osmd2 ON osm2.docid = osmd2.docid

                                -- Join com a ultima supervisão ativa para o tipo determinado
                                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 3 GROUP BY osmt.tomid, osmt.obrid ) AS osm3 ON osm3.obrid = o.obrid
                                LEFT JOIN workflow.documento osmd3 ON osm3.docid = osmd3.docid
                                
                                {$joinEmpresaMI}

                                LEFT JOIN (
                                            SELECT
                                                    max( res_estado ) as res_estado,
                                                    obrid
                                            FROM
                                            (
                                                SELECT DISTINCT
                                                        CASE
                                                                WHEN rstdtsuperacao IS NOT NULL THEN 1
                                                                WHEN rstdtprevisaoregularizacao > now() THEN 2
                                                                WHEN rstdtprevisaoregularizacao < now() THEN 3
                                                                ELSE 2
                                                        END as res_estado,
                                                        obrid
                                                FROM
                                                        obras2.restricao
                                                WHERE
                                                        obrid IS NOT NULL AND
                                                        rstitem = 'R' AND
                                                        rststatus = 'A'
                                            ) as  foo
                                            GROUP BY obrid
                                          ) res ON res.obrid = o.obrid
                                LEFT JOIN (
                                            SELECT
                                                    max( inc_estado ) as inc_estado,
                                                    obrid
                                            FROM
                                                    (
                                                        SELECT DISTINCT
                                                                CASE
                                                                        WHEN rstdtsuperacao IS NOT NULL THEN 1
                                                                        WHEN rstdtprevisaoregularizacao > now() THEN 2
                                                                        WHEN rstdtprevisaoregularizacao < now() THEN 3
                                                                        ELSE 2
                                                                END as inc_estado,
                                                                obrid
                                                        FROM
                                                                obras2.restricao
                                                        WHERE
                                                                obrid IS NOT NULL AND
                                                                rstitem = 'I' AND
                                                                rststatus = 'A'
                                                        ) as  foo
                                            GROUP BY obrid
                                          ) inc ON inc.obrid = o.obrid
                                ";
                break;
            default:
                $joinMi[] = " obras2.obras o
                                    INNER JOIN entidade.endereco       ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
                                    LEFT JOIN territorios.municipio 	mun ON mun.muncod = ende.muncod
                                    LEFT JOIN workflow.documento          d ON d.docid    = o.docid AND d.tpdid = " . TPDID_OBJETO . "
                                    LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid   = d.esdid
                                    LEFT JOIN entidade.entidade         ent ON ent.entid  = o.entid
                                    LEFT JOIN obras2.tipologiaobra 	tpo ON tpo.tpoid  = o.tpoid
                                    LEFT JOIN obras2.empreendimento 	  e ON e.empid    = o.empid AND e.empstatus = 'A'
                                    LEFT JOIN obras2.obrascontrato       oc ON oc.obrid   = o.obrid AND oc.ocrstatus = 'A'
                                    LEFT JOIN obras2.contrato c             ON c.crtid    = oc.crtid AND c.crtstatus = 'A'

                                    LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'

                                    -- Join com a ultima supervisão ativa para o tipo determinado
                                    LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 1 GROUP BY osmt.tomid, osmt.obrid ) AS osm1 ON osm1.obrid = o.obrid
                                    LEFT JOIN workflow.documento osmd1 ON osm1.docid = osmd1.docid

                                    -- Join com a ultima supervisão ativa para o tipo determinado
                                    LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 2 GROUP BY osmt.tomid, osmt.obrid ) AS osm2 ON osm2.obrid = o.obrid
                                    LEFT JOIN workflow.documento osmd2 ON osm2.docid = osmd2.docid

                                    -- Join com a ultima supervisão ativa para o tipo determinado
                                    LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 3 GROUP BY osmt.tomid, osmt.obrid ) AS osm3 ON osm3.obrid = o.obrid
                                    LEFT JOIN workflow.documento osmd3 ON osm3.docid = osmd3.docid

                                    {$joinEmpresaMI}
                                    LEFT JOIN (
                                                SELECT
                                                        max( res_estado ) as res_estado,
                                                        obrid
                                                FROM
                                                (
                                                    SELECT DISTINCT
                                                            CASE
                                                                    WHEN rstdtsuperacao IS NOT NULL THEN 1
                                                                    WHEN rstdtprevisaoregularizacao > now() THEN 2
                                                                    WHEN rstdtprevisaoregularizacao < now() THEN 3
                                                                    ELSE 2
                                                            END as res_estado,
                                                            obrid
                                                    FROM
                                                            obras2.restricao
                                                    WHERE
                                                            obrid IS NOT NULL AND
                                                            rstitem = 'R' AND
                                                            rststatus = 'A'
                                                    ) as  foo
                                                GROUP BY obrid
                                              ) res ON res.obrid = o.obrid
                                    LEFT JOIN (
                                                SELECT
                                                        max( inc_estado ) as inc_estado,
                                                        obrid
                                                FROM
                                                        (
                                                        SELECT DISTINCT
                                                                CASE
                                                                        WHEN rstdtsuperacao IS NOT NULL THEN 1
                                                                        WHEN rstdtprevisaoregularizacao > now() THEN 2
                                                                        WHEN rstdtprevisaoregularizacao < now() THEN 3
                                                                        ELSE 2
                                                                END as inc_estado,
                                                                obrid
                                                        FROM
                                                                obras2.restricao
                                                        WHERE
                                                                obrid IS NOT NULL AND
                                                                rstitem = 'I' AND
                                                                rststatus = 'A'
                                                        ) as  foo
                                                GROUP BY
                                                        obrid
                                              ) inc ON inc.obrid = o.obrid
                            ";
        }


        $joinMi[] = "
                -- Dados do Processo e Termo
                LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid
        ";

        return $joinMi;
    }

    public function listaMiSql(Array $param = array())
    {
        $where  = array();
        $join   = array();

        $where  = $this->filtroSql($param, $tipoSelecao);

        if (possui_perfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_ADMINISTRATIVO)) && !$this->testa_superuser()) {
            //Modificado para evitar de pegar Obras de outros estados que não o da empresa.
//            $where[] = "
//                        ende.estuf IN (SELECT euf.estuf FROM obras2.usuarioresponsabilidade urs
//                                            INNER JOIN obras2.empresami_uf euf ON urs.emiid = euf.emiid  AND euf.eufstatus = 'A'
//                                            WHERE  urs.rpustatus = 'A' AND urs.usucpf = '{$_SESSION['usucpf']}'
//                                            GROUP BY euf.estuf)";
            $where[] = "
                        emi.emiid IN (SELECT urs.emiid FROM obras2.usuarioresponsabilidade urs
                                            WHERE  urs.rpustatus = 'A' AND urs.usucpf = '{$_SESSION['usucpf']}'  
                                            GROUP BY urs.emiid)
                       ";
        }

        $join   = $this->joinSql($param);
        $joinMi = $this->joinSqlMi($param['tipoAcao']);
        $select = $this->selectSqlMi($param['tipoAcao']);

        $sql = "
          SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                       {$select}
                FROM
                  " . (count($joinMi) ? implode(" ", $joinMi) : "") . "
                  " . (count($join)   ? implode(" ", $join)   : "") . "
                WHERE
                  " . (count($where) ? implode(' AND ', $where) : "") . "
                ) as f ORDER BY f.id";
//        ver(simec_htmlentities($sql));
        return $sql;
    }

    public function listaSql(Array $param = array(), $tipoSelecao = null)
    {
        
        $where = array();
        $join  = array();
        $where = $this->filtroSql($param, $tipoSelecao);
        $join  = $this->joinSql($param);

        switch ($tipoSelecao) {
            case 'vinculoContrato':
                $acao = "'<center>
                                <input name=\"obrid[]\" id=\"obrid_' || o.obrid || '\" type=\"checkbox\" value=\"' || o.obrid || '\" onclick=\"vincularObra(this)\" >
                                <input name=\"obr_dsc_' || o.obrid || '\" id=\"obr_dsc_' || o.obrid || '\" type=\"hidden\" value=\"' || o.obrnome || '\">
                                <input name=\"obr_emp_' || o.obrid || '\" id=\"obr_emp_' || o.obrid || '\" type=\"hidden\" value=\"' || COALESCE(e.empdsc, '') || '\">
                          </center>'";

                $select = "{$acao} AS acao,
                           obrnome,
                           empdsc,
                           clodsc,
                           tobdesc,
                           obrvalorprevisto";

                $sql = "SELECT
                        {$select}
                        FROM
                                obras2.obras o
                             JOIN obras2.empreendimento      e ON e.empid    = o.empid AND e.empstatus = 'A'
                        LEFT JOIN obras2.tipoobra          tob ON tob.tobid  = o.tobid
                             JOIN obras2.orgao             org ON org.orgid  = e.orgid
                        LEFT JOIN obras2.classificacaoobra clo ON clo.cloid  = o.cloid
                        LEFT JOIN entidade.endereco       ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
                        LEFT JOIN territorios.municipio    mun ON mun.muncod = ende.muncod
                        
                        LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'
                        
                        " . (count($join) ? implode(" ", $join) : "") . "
                        WHERE
                                " . (count($where) ? implode(' AND ', $where) : "") . "
                        ORDER BY
                                2";

                break;
            default:

                $imgNovoObj = '';
                $imgExc = '';
                if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC))) {
                    /*
                     * Adonias falou para deixar liberado, por enquanto, para o FNDE poder cadastrar um novo obj vinculado
                     * quando a obra estiver paralisada, sem levar em consideração o subtipo (contrato cancelado e recindido)
                     */
                    $imgNovoObj = "' ||
                                    CASE WHEN ed.esdid = " . ESDID_OBJ_PARALISADO . " AND o.obrstatus != 'P' THEN
                                        '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/editar_nome.gif\"
                                                style=\"cursor: pointer\"
                                                onclick=\"javascript: novaObrVinculada(\'' || o.obrid || '\');\"
                                                title=\"Cadastrar nova obra vinculada\">'
                                    ELSE
                                            '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                                    END || '";

                    $imgExc = "<img
                                    align=\"absmiddle\"
                                    src=\"/imagens/excluir.gif\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    onclick=\"javascript: excluirObr(\'' || o.obrid || '\');\"
                                    title=\"Excluir Obra\">";

                    // Inativado pois estava gerando inconsistencias no banco
                    $imgExc = "";
                }

                $acao = "'<center><div style=\"width:100px\">
                                    {$imgNovoObj}
                                    <img
                                            align=\"absmiddle\"
                                            src=\"/imagens/edit_on.gif\"
                                            style=\"cursor: pointer\"
                                            onclick=\"javascript: abreListaSupervisaoFnde(\'' || o.obrid || '\',\'' || o.empid || '\');\"
                                            title=\"Supervisão FNDE\" />
                                    <img
                                            align=\"absmiddle\"
                                            src=\"/imagens/seriehistorica_ativa.gif\"
                                            style=\"cursor: pointer\"
                                            onclick=\"javascript: abreEvolucaoFinan(\'' || o.obrid || '\');\"
                                            title=\"Evolução Financeira\" />
                                    <img
                                            align=\"absmiddle\"
                                            src=\"/imagens/alterar.gif\"
                                            style=\"cursor: pointer\"
                                            onclick=\"javascript: alterarObr(\'' || o.obrid || '\');\"
                                            title=\"Alterar Obra\" />

                                            {$imgExc}
                              </div></center>'";

//                $imgAnexo = "CASE o.obrsndocumentos WHEN true
//                                    THEN
//                                        '<img
//                                                align=\"absmiddle\"
//                                                src=\"/imagens/obras/anexo.png\"
//                                                style=\"cursor: pointer; width:15px;\"
//                                                onclick=\"javascript: abreDocumentoObjeto(\'' || o.obrid || '\');\"
//                                                title=\"Anexos da Obra\">'
//                                     ELSE ''
//                             END";
//
//                $imgFoto = "CASE o.obrsnfotos WHEN true
//                                THEN
//                                    '<img
//                                            align=\"absmiddle\"
//                                            src=\"/imagens/cam_foto.gif\"
//                                            style=\"cursor: pointer; width:15px;\"
//                                            onclick=\"javascript: abreFotoObjeto(\'' || o.obrid || '\');\"
//                                            title=\"Fotos da Obra\">'
//                                 ELSE
//                                    ''
//                             END";
//
//                $imgPagamento = "CASE
//                                    WHEN o.preid IS NOT NULL THEN
//                                        '<center>
//                                                <a onmouseover=\"SuperTitleAjax(u+\'' || o.preid || '\',this)\" onmouseout=\"SuperTitleOff(this);\">
//                                                        <img src=\"/imagens/money_g.gif\" style=\"cursor:pointer; width:15px;\">
//                                                </a>
//                                         </center>'
//                                    ELSE
//                                            ''
//                                 END";

                $R = '\'<img title="Restrições \'|| CASE WHEN res_estado = 1 THEN \'Superadas\' WHEN res_estado = 2 THEN \'Pendentes\' ELSE \'em Atraso\' END || \'"
			 				 style="cursor:pointer; width:15px;"
							 onclick="abreRestricao(\' || o.obrid || \');"
							 src="/imagens/obras/atencao\'|| CASE WHEN res_estado = 1 THEN \'_verde\' WHEN res_estado = 2 THEN \'\' ELSE \'_vermelho\' END || \'.png">\'';

                $I = '\'<img title="Impedimentos \'|| CASE WHEN inc_estado = 1 THEN \'Superadas\' WHEN inc_estado = 2 THEN \'Pendentes\' ELSE \'em Atraso\' END || \'" style="cursor:pointer; width:15px;"
							 onclick="window.location.href = \'\'obras2.php?modulo=principal/listaRestricao&acao=O&obrid=\'|| o.obrid ||\'\'\';"
							 src="/imagens/obras/atencao\'|| CASE WHEN inc_estado = 1 THEN \'_verde\' WHEN inc_estado = 2 THEN \'\' ELSE \'_vermelho\' END || \'.png">\'';

//                $AD = '\'<img title="Aditivos da Obra"
//                              style="cursor:pointer;
//                              width:15px;"
//                              onclick="window.location.href = \'\'obras2.php?modulo=principal/cadObra&acao=A&obrid=\'|| o.obrid ||\'\'\';"
//                              src="/imagens/obras/check.png">\'';

                $obraVinculada = ' CASE WHEN (SELECT COUNT(*)
                                                FROM obras2.obras subobras
                                                WHERE
                                                  subobras.obrid = o.obrid
                                                --(subobras.obridpai IS NOT NULL)
                                                --OR
                                                AND (subobras.obridvinculado IS NOT NULL) limit 1
                                               ) > 0
                                        THEN \'<img title="Com obra vinculada" src="/imagens/check_p.gif"/>\'
                                        ELSE \'\'
                                        END ';

                $selectExcel = array();
                if ($param['excel']) {
                    $selectExcel['ocrdtordemservico'] = " TO_CHAR(oc.ocrdtordemservico, 'dd/mm/YYYY') AS ocrdtordemservico, ";
                    $selectExcel['tipoparalisacao'] = " sup.tpldsc, ";
                    $selectExcel['mesoregiao'] = " meso.mesdsc, ";
                    $selectExcel['programa'] = 'pf.prfdesc programa,';
                    $selectExcel['fonte'] = 'too.toodescricao fonte,';
                    $selectExcel['esfera'] = "CASE
                                                    WHEN e.empesfera = 'M' THEN 'Municipal'
                                                    WHEN e.empesfera = 'E' THEN 'Estadual'
                                                    WHEN e.empesfera = 'F' THEN 'Federal'
                                                    ELSE ''
                                              END as esfera,";
                    $selectExcel['termo'] = " dop.dopnumerodocumento, TO_CHAR(dop.dopdatainclusao, 'dd/mm/YYYY') dopdatainclusao,";
                    $selectExcel['situacao'] = " str.strdsc as situacao,";

                    $selectExcel['obrdtultvistoria'] = " to_char( o.obrdtultvistoria, 'DD/MM/YYYY' ) AS obrdtultvistoria,";
                    $selectExcel['qtddias'] = " DATE_PART( 'days', NOW() - o.obrdtultvistoria )::text AS qtddias,";

                    $selectExcel['processoobra'] = "CASE WHEN po1.pronumeroprocesso IS NOT NULL THEN
                                                        to_char(Replace(Replace(Replace( po1.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                                                    WHEN po2.pronumeroprocesso IS NOT NULL THEN
                                                        to_char(Replace(Replace(Replace(po2.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                                                    END as processoobra, ";

                    $selectExcel['ano_processo'] = " CASE WHEN po1.pronumeroprocesso IS NOT NULL THEN
                                                        substring(Replace(Replace(Replace( po1.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4)
                                                     WHEN po2.pronumeroprocesso IS NOT NULL THEN
                                                        substring(Replace(Replace(Replace( po2.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4)
                                                     END as ano_processo, ";
                }


                $select = "		{$acao} AS acao,
                                        CASE WHEN res_estado IS NOT NULL THEN $R ELSE '' END as r,
                                        CASE WHEN inc_estado IS NOT NULL THEN $I ELSE '' END as i,
                                        {$obraVinculada} as v,
                                        o.obrid as id,
                                        o.preid as preid,

                                        --CASE WHEN frpid = " . FRPID_CONVENIO . " THEN numconvenio ELSE '' END as convenio,
                                        --CASE WHEN frpid = " . FRPID_CONVENIO . " THEN obranoconvenio ELSE '' END as ano_convenio,
/*
                                        CASE WHEN o.obrnumprocessoconv IS NULL AND pf.prfid != 41 THEN
                                                cast ( (SELECT MAX(d.dopnumerodocumento)
                                                    from par.documentopar  d
                                                      inner join par.termocomposicao tc on tc.dopid = d.dopid
                                                      inner join obras.preobra pre on pre.preid = tc.preid
                                                      WHERE pre.preid = o.preid AND d.dopstatus = 'A'
                                                      GROUP BY pre.preid) as varchar)
                                        WHEN o.obrnumprocessoconv IS NULL AND pf.prfid = 41 THEN
                                            cast (  (SELECT 'PAC2 '||lpad(terid||'/'||to_char(terdatainclusao,'YYYY'),10,'0') 
                                            		FROM par.termocompromissopac
                                            		WHERE
                                            			terid = (SELECT MAX(ter.terid)
                                                            	FROM par.termocompromissopac ter
                                                            	LEFT JOIN par.termoobraspaccomposicao tob ON tob.terid = ter.terid
                                                            	WHERE tob.preid = o.preid AND ter.terstatus='A')) as varchar)

                                        WHEN o.obrnumprocessoconv IS NOT NULL AND o.tooid = 2 AND TRIM(o.obrnumprocessoconv) != '' THEN
                                             o.obrnumprocessoconv
                                        END as convenio,


                                        CASE WHEN o.obrnumprocessoconv IS NULL AND pf.prfid != 41 THEN
                                                cast ((SELECT d.dopano FROM (SELECT MAX(d.dopnumerodocumento), MAX(d.dopano) dopano
                                                    from par.documentopar  d
                                                      inner join par.termocomposicao tc on tc.dopid = d.dopid
                                                      inner join obras.preobra pre on pre.preid = tc.preid
                                                      WHERE pre.preid = o.preid AND d.dopstatus = 'A'
                                                      GROUP BY pre.preid, d.dopano) d) as varchar)
                                        WHEN o.obrnumprocessoconv IS NULL AND pf.prfid = 41 THEN
                                            cast ( (SELECT to_char(terdatainclusao,'YYYY')
                                            		FROM par.termocompromissopac
                                            		WHERE
                                            			terid = (SELECT MAX(ter.terid)
                                                            	FROM par.termocompromissopac ter
                                                            	LEFT JOIN par.termoobraspaccomposicao tob ON tob.terid = ter.terid
                                                            	WHERE tob.preid = o.preid AND ter.terstatus='A')) as varchar)
                                        WHEN o.tooid = 2 THEN
                                             o.obranoconvenio
                                        END as ano_convenio,
*/

                                        o.obrnumprocessoconv as numero_processo,
                                        o.numconvenio as numero_convenio,
                                        o.obranoconvenio as ano_convenio,


                                        {$selectExcel['processoobra']}

                                        {$selectExcel['ano_processo']}

                                        CASE WHEN e.prfid = 42 THEN
                                         '<b style=\"color:green\">(EP) </b>'
                                         ELSE
                                         ''
                                         END
                                            ||
                                        '<a href=\"javascript: alterarObr(\'' || o.obrid || '\');\">(' || o.obrid || ') ' || obrnome || '</a>'
                                         as descricao,
                                        {$selectExcel['ocrdtordemservico']}
                                        entnome as unidade_implantadora,
                                        {$selectExcel['mesoregiao']}
                                        {$selectExcel['termo']}
                                        mun.mundescricao as municipio,
                                        mun.estuf as uf,
                                        TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
                                        TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
                                        CASE
                                                WHEN str.strid = " . STRID_OBJ_CONCLUIDO . " THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || str.strdsc || '</font>'
                                                ELSE str.strdsc
                                        END as situacao,
                                        {$selectExcel['situacao']}
                                        {$selectExcel['tipoparalisacao']}
                                        --


                                        '<FONT ' ||
                                            CASE WHEN ed.esdid IN (693) THEN
                                                'COLOR=\"#3276B1\">' ||
                                                CASE WHEN obrdtultvistoria IS NOT NULL THEN
                                                        to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
                                                     ELSE
                                                        to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'
                                                END
                                            -- Se tiver ultima vistoria e a obra estiver paralisada ou execucao, aplica as cores
                                            WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN

                                                CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) < 45 THEN
                                                        'COLOR=\"#3CC03C\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
                                                     WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                                                        'COLOR=\"#FAD200\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                                                     WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 60 THEN
                                                        'COLOR=\"#FF0000\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">'
                                                END
                                                || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
                                            -- Se não tiver ultima vistoria e estiver em execução ou paralisada usa a data de inclusao, aplica as cores
                                            -- Quando MI traz a data do aceite
                                            WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) THEN

                                                CASE WHEN DATE_PART('days', NOW() - (CASE WHEN o.tpoid IN (104, 105) THEN (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1) ELSE obrdtinclusao END)) < 45 THEN
                                                        'COLOR=\"#3CC03C\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
                                                     WHEN DATE_PART('days', NOW() - (CASE WHEN o.tpoid IN (104, 105) THEN (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1) ELSE obrdtinclusao END)) >= 45 AND DATE_PART('days', NOW() - (CASE WHEN o.tpoid IN (104, 105) THEN (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1) ELSE obrdtinclusao END)) < 60 THEN
                                                        'COLOR=\"#FAD200\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                                                     WHEN DATE_PART('days', NOW() - (CASE WHEN o.tpoid IN (104, 105) THEN (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1) ELSE obrdtinclusao END)) >= 60 THEN
                                                        'COLOR=\"#FF0000\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">'
                                                END
                                                || to_char((CASE WHEN o.tpoid IN (104, 105) THEN (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1) ELSE obrdtinclusao END), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (CASE WHEN o.tpoid IN (104, 105) THEN (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1) ELSE obrdtinclusao END))||' dia(s) )'
                                            -- Se tiver vistoria e não esta em execucao ou paralisada, sem cores
                                            WHEN o.obrdtultvistoria IS NULL THEN
                                                CASE WHEN DATE_PART('days', NOW() - obrdtinclusao) < 45 THEN
                                                        'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) >= 45 AND DATE_PART('days', NOW() - obrdtinclusao) < 60 THEN
                                                        'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) >= 60 THEN
                                                        'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">'
                                                END
                                                || to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'

                                            -- Se tiver vistoria e não esta em execucao ou paralisada, sem cores
                                            ELSE

                                                CASE WHEN DATE_PART('days', NOW() - obrdtinclusao) <= 45 THEN
                                                        'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 45 AND DATE_PART('days', NOW() - obrdtinclusao) <= 60 THEN
                                                        'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                                                     WHEN DATE_PART('days', NOW() - obrdtinclusao) > 60 THEN
                                                        'COLOR=\"#000000\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">'
                                                END
                                                || to_char(obrdtinclusao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )'

                                            END
                                        || '</FONT>' AS ultima_atualizacao

                                        ,
                                        {$selectExcel['obrdtultvistoria']}
                                        {$selectExcel['qtddias']}

                                        --
                                        o.obrpercentultvistoria || '%' as obrpercentultvistoria,
                                        to_char( e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa,
                                        e.emppercentultvistoriaempresa || '%' as emppercentultvistoriaempresa,
                                        tpo.tpodsc,
                                        {$selectExcel['programa']}
                                        {$selectExcel['fonte']}
                                        {$selectExcel['esfera']}
                                        ocrvalorexecucao, o.obrvalorprevisto";

                $sql = "SELECT
                                DISTINCT
                                {$select}
                                FROM
                                        obras2.obras o
                                LEFT JOIN obras2.situacao_registro      str ON str.strid = o.strid
                                LEFT JOIN entidade.endereco            ende ON ende.endid = o.endid AND ende.endstatus = 'A'
                                LEFT JOIN territorios.municipio         mun ON mun.muncod = ende.muncod
                                LEFT JOIN workflow.documento              d ON d.docid    = o.docid
                                LEFT JOIN workflow.estadodocumento       ed ON ed.esdid   = d.esdid
                                LEFT JOIN entidade.entidade             ent ON ent.entid  = o.entid
                                LEFT JOIN obras2.tipologiaobra          tpo ON tpo.tpoid  = o.tpoid
                                LEFT JOIN obras2.empreendimento           e ON e.empid    = o.empid AND e.empstatus = 'A'
                                LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                                LEFT JOIN obras2.obrascontrato oc ON oc.ocrid = ocr.ocrid
                                LEFT JOIN obras2.programatipologia      ptp ON ptp.tpoid  = tpo.tpoid
                                LEFT JOIN obras2.programafonte          prf ON prf.prfid  = ptp.prfid
                                LEFT JOIN obras2.programafonte          pf ON pf.prfid    = e.prfid
                                LEFT JOIN obras2.tipoorigemobra         too ON o.tooid    = too.tooid

                                -- Informações do PAR
                                LEFT join obras.preobra                 pre ON pre.preid  = o.preid
                                LEFT JOIN par.processoobrasparcomposicao pop1 ON pop1.preid = o.preid AND pop1.pocstatus = 'A'
                                LEFT JOIN par.processoobraspar po1 on po1.proid = pop1.proid and po1.prostatus = 'A'
                                LEFT join par.documentopar              dop ON dop.proid  = po1.proid AND dop.dopstatus = 'A'
                                LEFT JOIN par.processoobraspaccomposicao pop2 ON pop2.preid = o.preid AND pop2.pocstatus = 'A'
                                LEFT JOIN par.processoobra po2 on po2.proid = pop2.proid and po2.prostatus = 'A'

                                LEFT JOIN obras2.evolucaomi            evmi ON evmi.obrid = o.obrid AND evmi.emistatus = 'A'

                                LEFT JOIN painel.dadosconvenios        dco ON dco.dcoprocesso = Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')

                                LEFT JOIN ( SELECT max( res_estado ) as res_estado, obrid
                                            FROM ( SELECT DISTINCT
                                                        CASE
                                                                WHEN rstdtsuperacao IS NOT NULL THEN 1
                                                                WHEN rstdtprevisaoregularizacao > now() THEN 2
                                                                WHEN rstdtprevisaoregularizacao < now() THEN 3
                                                                ELSE 2
                                                        END as res_estado,
                                                        obrid
                                                   FROM obras2.restricao
                                                   WHERE obrid IS NOT NULL AND rstitem = 'R' AND rststatus = 'A'
                                                    ) as  foo
                                                GROUP BY obrid
                                            ) res ON res.obrid = o.obrid

                                LEFT JOIN (
                                            SELECT
                                                    max( inc_estado ) as inc_estado,
                                                    obrid
                                            FROM
                                                    (
                                                    SELECT DISTINCT
                                                            CASE
                                                                    WHEN rstdtsuperacao IS NOT NULL THEN 1
                                                                    WHEN rstdtprevisaoregularizacao > now() THEN 2
                                                                    WHEN rstdtprevisaoregularizacao < now() THEN 3
                                                                    ELSE 2
                                                            END as inc_estado,
                                                            obrid
                                                    FROM
                                                            obras2.restricao
                                                    WHERE
                                                            obrid IS NOT NULL AND
                                                            rstitem = 'I' AND
                                                            rststatus = 'A'
                                                    ) as  foo
                                            GROUP BY
                                                    obrid
                                            ) inc ON inc.obrid = o.obrid

                                " . (count($join) ? implode(" ", $join) : "") . "
                                WHERE
                                        " . (count($where) ? implode(' AND ', $where) : "") . "

                                        --AND o.obrpercentultvistoria > 100
                                        --AND ed.esdid != 693

                                ORDER BY
                                        2";

                break;
        }
//        ver($sql);
        return $sql;
    }
// -- fim da listasql


    public function listaDadosResumo(Array $param = array())
    {
        $where = array();
        $join = array();

        if ($param['not(obridpai)']) {
            $where[] = "o.obridpai IS NULL";
        }

        if ($param['empid']) {
            $param['empid'] = (array) $param['empid'];
            $where[] = "o.empid IN(" . implode(", ", $param['empid']) . ")";
        }

        $sql = "SELECT
                        o.obrid,
                        o.obrnome,
                        TO_CHAR(o.obrdtultvistoria, 'dd/mm/YYYY') AS obrdtultvistoria,
                        o.obrpercentultvistoria
                FROM
                        obras2.obras o
                " . (count($join) ? implode(" ", $join) : "") . "
                WHERE
                        o.obrstatus = 'A'
                        " . (count($where) ? " AND " . implode(' AND ', $where) : "") . "
                ORDER BY
                        obrnome";

        $dado = $this->carregar($sql);
        return ($dado ? $dado : array());
    }

    public function pegaObrasConvenio($numconvenio, $obranoconvenio)
    {
        $sql = "SELECT
                        *
                FROM
                        obras2.obras o
                WHERE o.obrstatus = 'A' AND o.obridpai IS NULL AND o.numconvenio = '{$numconvenio}' AND o.obranoconvenio = '{$obranoconvenio}' AND tooid = 2";

        $dado = $this->carregar($sql);
        return ($dado ? $dado : array());
    }

    public function listaObraConvenio(Array $param = array())
    {

        $where = array("oi.obrstatus = 'A'");

        if ($param['numconvenio']) {
            $param['numconvenio'] = (array) $param['numconvenio'];
            $where[] = "oi.numconvenio IN('" . implode("', '", $param['numconvenio']) . "')";
        } else {
            return array();
        }

        $sql = "SELECT
                        '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abrirobra(' || oi.obrid || ')\"/>' as acao,
                        oi.obrid,
                        '<a href=\"javascript:void(0)\" onclick=\"abrirobra(' || oi.obrid || ')\" style=\"color:black\">' || oi.obrnome || '</a>',
                        ee.entnome,
                        sa.stadesc,
                        oi.obrpercentultvistoria
                FROM
                        obras2.obras oi
                INNER JOIN
                        entidade.entidade ee ON ee.entid = oi.entid
                LEFT JOIN
                        obras2.situacaoatividade sa ON sa.staid = oi.staid AND
                                                                                   sa.stastatus = 'A'
                WHERE
                        " . (count($where) ? implode(' AND ', $where) : "");

        return $sql;

        //		$dados = $this->carregar( $sql );
        //		return ($dados ? $dados : array());
    }

    public function recuperaContatos()
    {
        $sql = "SELECT
                        c.tpcid as tipo,
                        c.cobid as responsavel,
                        c.entid as entidade,
                        e.entnome as nome,
                        e.entnumcpfcnpj as cpf
                FROM
                        obras2.contatos_obra c
                INNER JOIN entidade.entidade e ON e.entid = c.entid
                WHERE
                        c.obrid = '" . $this->obrid . "'
                        AND c.cobstatus = 'A'
                        AND e.entstatus != 'I'";
        return $this->carregar($sql);
    }

    public function recuperaResponsaveis()
    {

        if ($this->recuperaOrgid() == ORGID_EDUCACAO_BASICA) {
            $filtro = " AND ur.pflcod IN (" . PFLCOD_SUPERVISOR_UNIDADE . ", " . PFLCOD_GESTOR_UNIDADE . ", " . PFLCOD_EMPRESA_CONTRATADA . ")";
        } else {
            $filtro = " AND ur.pflcod IN (" . PFLCOD_EMPRESA_CONTRATADA . ")";
        }

        $sql = "SELECT DISTINCT
                        su.usucpf as cpf,
                        su.usunome as nome
                FROM
                        seguranca.usuario su
                INNER JOIN obras2.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf AND ur.rpustatus = 'A'
                WHERE
                        ur.obrid = {$this->obrid}
                        {$filtro}
                ORDER BY
                        su.usunome";

        return $this->carregar($sql);
    }

    public function limparResponsaveisObra()
    {

        $sql = "UPDATE obras2.usuarioresponsabilidade SET
    				rpustatus = 'I'
    			WHERE
    				obrid = " . $this->obrid;
        $this->executar($sql);
        $this->commit();
    }

    public function recuperaOrgid()
    {

        $empreendimento = new Empreendimento($this->empid);
        return $empreendimento->orgid;
    }

    public function pegaContratoObra($obrid = null) 
    {
        $obrid = ($obrid ? $obrid : $this->obrid);

        $sql = "SELECT DISTINCT
    				ocrid
    			FROM
    				obras2.obrascontrato
    			WHERE
    				ocrstatus = 'A'
    				AND obrid = " . $obrid;

        return $this->pegaUm($sql);
    }

    public function pegaContratoPorObra($obrid = null)
    {
        $obrid = ($obrid ? $obrid : $this->obrid);

        $sql = "SELECT DISTINCT
                        crtid
                FROM
                        obras2.obrascontrato
                WHERE
                        ocrstatus = 'A'
                        AND obrid = " . $obrid;

        return $this->pegaUm($sql);
    }

    public function getDadosCabecalho($obrid)
    {
        if(empty($obrid)){
            return array();
        }
        $sql = "SELECT
                        o.obrid,
                        o.obrnome,
                        e.empid,
                        e.empdsc,
                        og.orgdesc,
                        ed.estuf,
                        m.mundescricao,
                        m.muncod,
                        po.preid,
                        po.predescricao,
                        po.muncod AS premuncod,
                        d.demid,
                        d.demnome,
                        o.obrid_1,
                        e.empesfera
                FROM
                obras2.obras o
                JOIN obras2.empreendimento e ON e.empid = o.empid AND
                                                                   e.empstatus = 'A'
                JOIN obras2.orgao og ON og.orgid = e.orgid
                LEFT JOIN entidade.endereco ed ON ed.endid = e.endid
                LEFT JOIN territorios.municipio m ON m.muncod = ed.muncod
                LEFT JOIN obras.preobra po ON po.preid = e.preid
                LEFT JOIN obras2.demanda d ON d.demid = e.demid
                        WHERE
                o.obrid = {$obrid}
                 ";
//                --Modificado para atender os casos das obras vinculadas cujo o obrstatus = 'P'
//                AND o.obrstatus = 'A'
//                 ";
        $dados = $this->pegaLinha($sql);
        return ($dados ? $dados : array());
    }

    public function excluirImagem($obrid, $arqid)
    {
        $sql = "UPDATE obras2.obras
	    		SET arqid=NULL
	    		WHERE obrid='{$obrid}'";
        $this->executar($sql);

        // Tive que tirar a exclusão do arquivo devido as replicas do empreendimento
        //	    include_once APPRAIZ."includes/classes/fileSimec.class.inc";
        //
		//	    $file       = new FilesSimec("obras", array(true), "obras2");
        //	    $file->setPulaTableEschema( true );
        //	    $file->setPasta('obras2');
        //	    $file->setRemoveUpload($arqid);
    }

    public function listaComboPopupSql(Array $where = array())
    {
        $arJoin = array();
        $arWhere = array();

        if (isset($where['naoLicitada'])) {
            $arWhere[] = "obrid NOT IN(SELECT
                                            o.obrid
                                       FROM
                                            obras2.obras o
                                       JOIN obras2.obralicitacao ol ON ol.obrid = o.obrid AND
                                                    " . ( is_numeric($where['naoLicitada']) ? "ol.licid != {$where['naoLicitada']} AND" : "" ) . "
                                                    ol.oblstatus = 'A'
                                       WHERE
                                            obrstatus = 'A')";
        }

        if (isset($where['orgid'])) {
            $arJoin['empreendimento'] = "JOIN obras2.empreendimento e ON e.empid = o.empid AND
																		 e.orgid = {$where['orgid']}";
        }

        if ($where['not(obridpai)']) {
            $arWhere[] = "o.obridpai IS NULL";
        }

        $sql = "SELECT
                                obrid AS codigo,
                                obrnome AS descricao
                        FROM
                                obras2.obras o
                        " . (count($arJoin) ? implode(" ", $arJoin) : "") . "
                        WHERE
                                obrstatus = 'A'" .
                (count($arWhere) ? " AND " . implode(" AND ", $arWhere) : "");

        return $sql;
    }

    public function carregaComboPopupPorLicitacao($licid)
    {
        if (empty($licid)) {
            return array();
        }

        $sql = "SELECT
                                o.obrid AS codigo,
                                o.obrnome AS descricao
                        FROM
                                obras2.obralicitacao ol
                        JOIN obras2.obras o ON o.obrid = ol.obrid AND
                                                                   o.obrstatus = 'A'
                        WHERE
                                ol.oblstatus = 'A' AND
                                ol.licid = {$licid}";

        $dados = $this->carregar($sql);

        return $dados;
    }

    public function carregaComboPopupPorObra($obrid)
    {
        if (empty($obrid)) {
            return array();
        }

        $sql = "SELECT
                        o.obrid AS codigo,
                        o.obrnome AS descricao
                FROM
                        obras2.obras o
                WHERE
                        o.obrstatus = 'A' AND
                        o.obrid = {$obrid}";

        $dados = $this->carregar($sql);

        return $dados;
    }

    public function pegaIdObraPorEmpid($empid, Array $param = array())
    {
        $arWhere = array();

        if ($param['not(obridpai)']) {
            $arWhere[] = "obridpai IS NULL";
        }

        $sql = "SELECT
                        obrid
                FROM
                        obras2.obras
                WHERE
                        empid = {$empid} AND
                        " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                        obrstatus = 'A'
                ORDER BY
                        obrid";

        $dado = $this->carregarColuna($sql);
        return ($dado ? $dado : array());
    }

    public function pegaIdObraEVinculadasPorEmpid($empid, Array $param = array())
    {
        $arWhere = array();


        $sql = "SELECT
                        obrid
                FROM
                        obras2.obras
                WHERE
                        empid = {$empid} AND
                        obridpai IS NULL AND
                        obrstatus IN ('A', 'P')
                ORDER BY
                        obrid";

        $dado = $this->carregarColuna($sql);
        return ($dado ? $dado : array());
    }

    public function pegaObraPorEmpid($empid, Array $param = array())
    {
        $arWhere = array();
        $arWhere[] = "obridpai IS NULL";
        $sql = "SELECT
                        *
                FROM
                        obras2.obras
                WHERE
                        empid={$empid} AND
                        " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                        obrstatus = 'A'
                ORDER BY
                        obrid";
        $dado = $this->pegaLinha($sql);
        return ($dado ? $dado : array());
    }
    
    public function pegaUltimaObraPorEmpId($empid)
    {
        $sql = "SELECT * FROM obras2.obras
                WHERE
                    empid={$empid} AND
                    obrstatus = 'A'
                ORDER BY
                    obrid DESC LIMIT 1";
        $dado = $this->pegaLinha($sql);
        return ($dado ? $dado : array());
    }

    public function pegaQtdObraParalisadaPorEmpid($empid)
    {
        $sql = "SELECT
                        COUNT(o.*) AS total
                FROM
                        obras2.obras o
                JOIN workflow.documento d ON d.docid = o.docid AND d.esdid = " . ESDID_OBJ_PARALISACAO . "
                WHERE
                        o.obrstatus = 'A' AND
                        empid = {$empid}";

        return $this->pegaUm($sql);
    }

    public function listaCombo($empid)
    {
        $sql = "SELECT
                        obrid AS codigo,
                        obrnome AS descricao
                FROM
                        obras2.obras
                WHERE
                        obrstatus = 'A' AND
                        obridpai IS NULL AND
                        empid = {$empid}";

        $dados = $this->carregar($sql);
        return (is_array($dados) ? $dados : array());
    }

    public function pegaUltDataMudancaEstadoWf($obrid)
    {
        /*
         * Preciso deste campo sem formatação, caso seja necessário mudar crie parâmetro e mantenha o padrão como está.
         */
        $sql = "SELECT
                        MAX(h.hstid) AS hstid,
                        h.htddata
                FROM
                        obras2.obras o
                JOIN workflow.documento d ON d.docid = o.docid
                JOIN workflow.historicodocumento h ON h.docid = d.docid
                WHERE
                        o.obrid = {$obrid}
                GROUP BY
                        htddata";
        $htddata = $this->pegaUm($sql, 'htddata');

        return $htddata;
    }
    
    public function getEstadoObraWf($obrid = NULL)
    {
        if (empty($obrid)){
            $obrid = $this->obrid;
        }
        
        $sql = "SELECT
                        d.*, esd.*, h.*
                FROM
                        obras2.obras o
                JOIN workflow.documento            d ON d.docid    = o.docid
                JOIN workflow.estadodocumento    esd ON esd.esdid  = d.esdid
                JOIN workflow.historicodocumento   h ON h.docid    = d.docid
                WHERE
                        o.obrid = {$obrid}
                ORDER BY h.hstid desc
                LIMIT 1
                ";
        
        $estadoObra = $this->pegaLinha($sql);
        $estadoObra = (empty($estadoObra)) ? array() : $estadoObra;

        return $estadoObra;
    }

    public function getEnderecoObra($obrid)
    {
        $sql = " SELECT e.* FROM obras2.obras o
                 JOIN entidade.endereco e ON o.endid = e.endid AND e.endstatus = 'A'
                 WHERE o.obrid = {$obrid}";

        return $this->pegaLinha($sql);
    }

    public function exportarCronogramaPadraoParaObra($obrid = null)
    {

        // Itens Obra
        $itensObra = new ItensComposicaoObras();

        // Obra
        $this->obrid = ($obrid) ? $obrid : $this->obrid;
        $this->carregarPorId($this->obrid);

        // Endereçp
        $end = $this->getEnderecoObra($this->obrid);

        // OS Execução
        $ordemservico = new OrdemServicoMI();
        $os = current($ordemservico->recuperarTodos("*", array("obrid = ".$this->obrid, "tomid = 1" ) ));

        // Cronograma
        $cronogramaObra = new Cronograma();
        $cronogramaObra->cronogramaObra($this->obrid);

        $cronogramaPadrao = new Cronograma_PadraoMi();
        $cronograma = $cronogramaPadrao->pegaCronogramaPadrao($end['estuf'], $this->tpoid);

        // Itens Cronograma
        $itensComposicao = new Itens_Composicao_PadraoMi();
        $itens = $itensComposicao->pegaTudoPorCpm($cronograma['cpmid']);

        $dataInicio = $os['osmdtinicio'];

        foreach($itens as $item){

            $qtdItens = new QtdItensComposicaoObraMi();
            $qtdItem = $qtdItens->pegaPorObridEIcmid($this->obrid, $item['icmid']);

            $dados = $itensObra->getByObraAndItens($this->obrid, $item['itcid']);

            $itemComposicaoObra = new ItensComposicaoObras();
            $dados = ($dados) ? $dados : array();

            $dados['obrid'] =  $this->obrid;

            $dados['itcid'] =  $item['itcid'];
            $dados['icovlritem'] =  $item['icmvalor'];
            $dados['icodtinicioitem'] = (!empty($item['icmdiasinicio'])) ?  date('Y-m-d', strtotime($dataInicio . ' + ' . $item['icmdiasinicio'] . ' days')) : $dataInicio;
            $dados['icodterminoitem'] = (!empty($item['icmdiasduracao'])) ? date('Y-m-d', strtotime($dataInicio . ' + ' . $item['icmdiasduracao'] . ' days')) : $dataInicio;
            $dados['icodtinclusao'] = 'NOW()';
            $dados['icoordem'] = $item['icmordem'];
            $dados['icostatus'] = 'A';
            $dados['icovigente'] = 'A';
            $dados['relativoedificacao'] = 'D';
            $dados['croid'] = $cronogramaObra->croid;

            if($item['relativoedificacao'] == 'F'){
                $dados['itcquantidade'] = ($qtdItem['qioquantidade']) ? $qtdItem['qioquantidade'] : 0;
                $dados['relativoedificacao'] = $item['relativoedificacao'];
                $dados['umdid'] = $item['umdid'];
            }
            $itemComposicaoObra->popularDadosObjeto($dados);
            $itemComposicaoObra->salvar();
            $itemComposicaoObra->commit();
        }
        return true;
    }

    public function atualizaTravaCronograma($obrid, $flag = 'TRUE')
    {

        if($flag == 'TRUE' || $flag == TRUE){
           $flag = 't';
        }else{
           $flag = 'f';
        }

        $sql = "UPDATE obras2.obras SET
                       obrtravaedicaocronograma = '".$flag."'
                WHERE
                       obrid = " . $obrid;
        $this->executar($sql);
        $this->commit();

    }

    public function getTravaCronograma($obrid){
        $sql = "SELECT obrtravaedicaocronograma FROM obras2.obras
                WHERE obrid = " . $obrid;
        return $this->pegaUm($sql);
    }

    public function getPercentExecutadoAproveitadoObraAnterior($obrid = null){
        $obrid = ($obrid == null) ? ($_SESSION['obras2']['obrid'] != '') ? $_SESSION['obras2']['obrid'] : ($this->obrid != '') ? $this->obrid : $obrid : $obrid ;

        if($obrid != NULL){
            $sql = "SELECT obrid, obrperccontratoanterior, obridvinculado, obrpercentultvistoria FROM obras2.obras WHERE obrid = '" . $obrid . "'";
            $dadosObr = $this->pegaLinha($sql);
        }else{
            $dadosObr = false;
        }
        return $dadosObr;
    }

    public function getLinhaTabelaPercentExecutado($obrid = null){
        $htm   = '';
        $dados = $this->getPercentExecutadoAproveitadoObraAnterior($obrid);
        if($dados != false && $obrid != null){
            $obrperccontratoanterior = $dados['obrperccontratoanterior'];
            $obrpercentultvistoria   = $dados['obrpercentultvistoria'];
            $obridvinculado          = $dados['obridvinculado'];
            $percentual_final        = $this->calculaPercentualValidacaoParcela($obrid);
        }else{
            $obrperccontratoanterior = NULL;
            $obridvinculado          = NULL;
        }
        if( ($obrperccontratoanterior != '' || $obrperccontratoanterior != NULL) && ($obridvinculado != '' || $obridvinculado != NULL) ){
            //*
            $htm .= '<tr>
                        <td class="SubTituloDireita" width="20%"><b>Percentual executado do contrato atual (%):</b></td>
                        <td>
                            '.number_format($obrpercentultvistoria, 2, ',', '.').'%
                        </td>
                     </tr>';
            $htm .= '<tr>
                        <td class="SubTituloDireita" width="20%"><b>Percentual executado aproveitável do contrato anterior (%):</b></td>
                        <td>
                            '.number_format($obrperccontratoanterior, 2, ',', '.').'%
                        </td>
                     </tr>';
            //*
            $htm .= '<tr>
                        <td class="SubTituloDireita" width="20%"><b>Percentual executado somando o Anterior mais o Atual (%):</b></td>
                        <td>
                            '.number_format($percentual_final, 2, ',', '.').'%
                        </td>
                     </tr>';
            //* Linhas adicionadas para atender a demanda #248706. 
        }
        return $htm;
    }
    
    public function calculaPercentualValidacaoParcela($obrid = null){
        if($obrid == null){ $obrid = $this->obrid; if($obrid == null){ return 0; } }        
        if($this->obrid == null){ $this->carregarPorId($obrid); }        
        //Carregar o objeto com o obrid para que os atrubutos sejam carregados;
        $dadosPercentual         = $this->getPercentExecutadoAproveitadoObraAnterior($this->obrid);
        $percentual_aproveitavel = ($dadosPercentual) ? $dadosPercentual['obrperccontratoanterior'] : 0;
        $percentual_vistoria     = $this->obrpercentultvistoria;
        $percentual_do_todo      = 100 - $percentual_aproveitavel;
        $percentual_final        = round((($percentual_vistoria/100)*$percentual_do_todo), 2)+$percentual_aproveitavel;
        return $percentual_final;
    }

    public function getEstadoObra($obrid = null) {
        if($obrid == null){
            $obrid = $this->obrid;
            if($obrid == null){
                return array();
            }
        }

        $select = 'o.obrid, ed.esdid, ed.esddsc';

        $sql = "SELECT
            DISTINCT
                {$select}
            FROM
                obras2.obras o
            LEFT JOIN entidade.endereco      ende ON ende.endid     = o.endid AND
                                                     ende.endstatus = 'A' AND
                                                     ende.tpeid     = " . TIPO_ENDERECO_OBJETO . "
            LEFT JOIN territorios.municipio   mun ON mun.muncod     = ende.muncod
            LEFT JOIN workflow.documento        d ON d.docid        = o.docid AND tpdid = ".TPDID_OBJETO."
            LEFT JOIN workflow.estadodocumento ed ON ed.esdid       = d.esdid
            LEFT JOIN obras2.empreendimento     e ON e.empid        = o.empid AND e.empstatus = 'A'

            WHERE
                o.obrstatus = 'A' AND o.obridpai IS NULL AND e.orgid IN(3) AND o.obrid = {$obrid}
            ORDER BY
                2";
        $dados = $this->pegaLinha($sql);
        $dados = ($dados) ? $dados : array();
        return $dados;
    }
    
    public function atualizaPercentualVistoriaObra($obrid = null, $percent = -1) {
        
        if($obrid == null || trim($obrid) == ''){
            $obrid = $this->obrid;
            if($obrid == null){
                return false;
            }
        }
        if($percent == -1 || $percent == false){
            return false;
        }
        
        $sql = 'UPDATE obras2.obras SET obrpercentultvistoria = '.$percent.' WHERE obrid = '.$obrid;
        
        try{
            $this->executar($sql);
            return true;
        } catch (Exception $ex) {
            return false;
        }
        
    }
    
    public function getPagamentoPar($obrid)
    {
        $sql = "
            SELECT
                  *
                FROM
                  (SELECT DISTINCT
                      CASE WHEN em.prfid != 41 THEN
                            cast ( (SELECT MAX(d.dopnumerodocumento)
                                from par.documentopar  d
                                  inner join par.termocomposicao tc on tc.dopid = d.dopid
                                  inner join obras.preobra pre on pre.preid = tc.preid
                                  inner join obras2.obras o on o.preid = pre.preid
                                  WHERE pre.preid = obr.preid AND d.dopstatus = 'A'
                                  GROUP BY pre.preid) as varchar)
                        ELSE
                                  cast ( (SELECT MAX(ter.terid || '/' || TO_CHAR(ter.terdatainclusao, 'YYYY'))
                                        FROM par.termocompromissopac ter
                                        LEFT JOIN par.termoobra tob ON tob.terid = ter.terid
                                        WHERE tob.terid = obr.preid AND ter.terstatus='A') as varchar)
                        END as termocompromisso,

                    to_char(p.pronumeroprocesso :: BIGINT, 'FM00000\".\"000000\"/\"0000\"-\"00') AS \"Número processo\",

                    emp.empnumero AS \"Número empenho\",

                    pob.popvalorpagamento AS \"Valor da Parcela\",

                      CASE WHEN pre.prevalorobra > 0 THEN
                             ROUND(((SELECT SUM(popvalorpagamento)
                               FROM par.pagamentoobrapar p2
                               INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid
                               WHERE p2.preid = pre.preid AND pag2.pagid = pa.pagid AND pag2.pagstatus = 'A') / pre.prevalorobra )*100)
                             ELSE 0
                       END AS \"Percentual pagamento\",


                    pa.pagparcela AS \"Parcela\",

                    to_char(pa.pagdatapagamento, 'DD/MM/YYYY') AS \"Data pagamento\",

                    pa.pagsituacaopagamento AS situacao,

                    TRUNC(pre.prevalorobra, 2) AS \"Valor da Obra\",


                    (SELECT
                      COALESCE(sum(po.popvalorpagamento), 0.00)
                      FROM
                        par.pagamento pg
                      INNER JOIN par.empenho em ON em.empid = pg.empid and empstatus = 'A'
                      INNER JOIN par.pagamentoobrapar po ON po.pagid = pg.pagid
                      WHERE
                        em.empnumeroprocesso = p.pronumeroprocesso AND po.preid = pre.preid AND pg.pagstatus = 'A' AND pg.pagsituacaopagamento <> 'CANCELADO'
                    ) AS \"Valor total pago da obra\",


                    (
                         SELECT
                                     SUM(ROUND(((SELECT SUM(popvalorpagamento)
                                           FROM par.pagamentoobrapar p2
                                           INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid
                                           WHERE p2.preid = pre.preid AND pag2.pagid = po.pagid AND pag2.pagstatus = 'A') / pre.prevalorobra )*100)) as soma
                                      FROM
                                    par.pagamento pg
                                      INNER JOIN par.empenho em ON em.empid = pg.empid and empstatus = 'A'
                                      INNER JOIN par.pagamentoobrapar po ON po.pagid = pg.pagid
                                      WHERE
                                    em.empnumeroprocesso = '23400004307201214' AND po.preid = 26701
                         AND pg.pagstatus = 'A' AND pg.pagsituacaopagamento <> 'CANCELADO') AS \"Percentual total pago da obra\",
                         pa.pagnumeroob

                    FROM par.pagamentoobrapar pob
                        INNER JOIN par.processoobrasparcomposicao poc ON poc.preid  = pob.preid and poc.pocstatus = 'A'
                        INNER JOIN par.processoobraspar             p ON poc.proid  = p.proid  and p.prostatus = 'A'
                        INNER JOIN par.empenho                    emp ON p.pronumeroprocesso = emp.empnumeroprocesso AND emp.empsituacao <> 'CANCELADO' and empstatus = 'A'
                        INNER JOIN par.pagamento                    pa ON pa.empid   = emp.empid AND pob.pagid = pa.pagid
                        INNER JOIN obras.preobra                  pre ON pre.preid  = poc.preid
                        LEFT JOIN obras2.obras                    obr ON obr.preid  = pre.preid AND (obr.obridpai IS NULL)
                        LEFT JOIN obras2.empreendimento            em ON obr.empid  = em.empid
                        LEFT JOIN obras2.validacao                val ON val.obrid  = obr.obrid
                        LEFT JOIN seguranca.usuario               usu ON usu.usucpf = pa.usucpf
                        LEFT JOIN territorios.municipio             m ON m.muncod   = pre.muncod
                        LEFT JOIN par.documentopar                 dp ON dp.proid   = p.proid AND dopstatus = 'A'
                        LEFT JOIN territorios.estado                e ON e.estuf    = p.estuf
                        LEFT JOIN territorios.muntipomunicipio     mt ON mt.muncod  = m.muncod
                        LEFT JOIN territorios.tipomunicipio        tm ON tm.tpmid   = mt.tpmid
                        LEFT JOIN par.instrumentounidadeentidade  iue ON iue.inuid  = p.inuid AND iue.iuestatus = 'A' AND iue.iuedefault = TRUE
                        LEFT JOIN par.instrumentounidade           iu ON iu.inuid   = p.inuid
                        LEFT JOIN workflow.documento                d ON d.docid    = iu.docid AND d.tpdid = 44
                        LEFT JOIN workflow.estadodocumento         ed ON ed.esdid   = d.esdid
                    WHERE (pa.pagsituacaopagamento IS NULL OR pa.pagsituacaopagamento <> 'CANCELADO') AND obr.obrid = {$obrid}  AND pa.pagstatus = 'A'
                        
                UNION ALL
                
                  SELECT DISTINCT

                  CASE WHEN em.prfid != 41 THEN
                        cast ( (SELECT MAX(d.dopnumerodocumento)
                            from par.documentopar  d
                              inner join par.termocomposicao tc on tc.dopid = d.dopid
                              inner join obras.preobra pre on pre.preid = tc.preid
                              inner join obras2.obras o on o.preid = pre.preid
                              WHERE pre.preid = obr.preid AND d.dopstatus = 'A'
                              GROUP BY pre.preid) as varchar)
                    ELSE
                              cast ( (SELECT MAX(ter.terid || '/' || TO_CHAR(ter.terdatainclusao, 'YYYY'))
                                    FROM par.termocompromissopac ter
                                    LEFT JOIN par.termoobra tob ON tob.terid = ter.terid
                                    WHERE tob.terid = obr.preid AND ter.terstatus='A') as varchar)
                    END as termocompromisso,
                    to_char(p.pronumeroprocesso :: BIGINT, 'FM00000\".\"000000\"/\"0000\"-\"00') AS \"Número processo\",
                    emp.empnumero AS \"Número empenho\",
                    pob.pobvalorpagamento AS \"Valor da Parcela\",
                    CASE WHEN pre.prevalorobra > 0 THEN
                        ROUND(( (

                            (SELECT
                                SUM(pobvalorpagamento) AS perc_pag
                                  FROM par.pagamentoobra pao
                                  INNER JOIN par.pagamento pag2 ON pag2.pagid = pao.pagid AND pag2.pagstatus = 'A'
                                  WHERE pao.pagid = pa.pagid AND pa.pagsituacaopagamento <> 'CANCELADO' AND pao.preid = poc.preid)

                        ) / pre.prevalorobra )*100)
                        ELSE 0
                    END AS \"Percentual pagamento\",
                    pa.pagparcela AS \"Parcela\",
                    to_char(pa.pagdatapagamento, 'DD/MM/YYYY') AS \"Data pagamento\",
                    pa.pagsituacaopagamento AS situacao,
                    TRUNC(pre.prevalorobra, 2) AS \"Valor da Obra\",
                    (SELECT
                      COALESCE(sum(po.pobvalorpagamento), 0.00)
                      FROM
                        par.pagamento pg
                      INNER JOIN par.empenho em ON em.empid = pg.empid and empstatus = 'A'
                      INNER JOIN par.pagamentoobra po ON po.pagid = pg.pagid
                      WHERE em.empnumeroprocesso = p.pronumeroprocesso AND po.preid = pre.preid AND pg.pagstatus = 'A' AND pg.pagsituacaopagamento <> 'CANCELADO'
                    ) AS \"Valor total pago da obra\",
                    (SELECT SUM(pobpercentualpag) AS perc_pag
                      FROM 
                          par.pagamentoobra pao 
                      INNER JOIN par.pagamento pag1 ON pag1.pagid = pao.pagid AND pag1.pagsituacaopagamento <> 'CANCELADO'
                      WHERE pao.preid = poc.preid AND pag1.pagstatus = 'A'
                    ) AS \"Percentual total pago da obra\",
                    pa.pagnumeroob

                  FROM par.pagamentoobra pob
                      INNER JOIN par.processoobraspaccomposicao poc ON poc.preid  = pob.preid and poc.pocstatus = 'A'
                      INNER JOIN par.processoobra                 p ON poc.proid  = p.proid
                      INNER JOIN par.empenho                    emp ON p.pronumeroprocesso = emp.empnumeroprocesso AND emp.empsituacao <> 'CANCELADO' and empstatus = 'A'
                      INNER JOIN par.pagamento                   pa ON pa.empid   = emp.empid AND pob.pagid = pa.pagid AND pa.pagstatus = 'A'
                      INNER JOIN obras.preobra                  pre ON pre.preid  = poc.preid
                      LEFT JOIN obras2.obras                    obr ON obr.preid  = pre.preid AND (obr.obridpai IS NULL)
                      LEFT JOIN obras2.validacao                val ON val.obrid  = obr.obrid
                      LEFT JOIN seguranca.usuario               usu ON usu.usucpf = pa.usucpf
                      LEFT JOIN obras2.empreendimento            em ON obr.empid  = em.empid
                      LEFT JOIN territorios.municipio             m ON m.muncod   = pre.muncod
                      LEFT JOIN par.termocompromissopac          tc ON tc.proid   = p.proid AND terstatus = 'A'
                      LEFT JOIN territorios.estado                e ON e.estuf    = p.estuf
                      LEFT JOIN territorios.muntipomunicipio     mt ON mt.muncod  = m.muncod
                      LEFT JOIN territorios.tipomunicipio        tm ON tm.tpmid   = mt.tpmid
                  WHERE (pa.pagsituacaopagamento IS NULL OR pa.pagsituacaopagamento <> 'CANCELADO') AND obr.obrid = {$obrid}  AND pa.pagstatus = 'A'
                ) AS foo
        ";
                  
//        ver(simec_htmlentities($sql));
        
        return $this->carregar($sql);
    }

    public function verificaAprovacaoCondicional($obrid)
    {
        if(empty($obrid)){
            return false;
        }
        $sql = "select ob.obrid from obras2.obras ob
                inner join obras.preobra pre on pre.preid = ob.preid
                where obrstatus = 'A' and obridpai is null and ob.obrid = {$obrid} and
                (pre.docid in (select docid from workflow.documento where esdid = 889)
                or pre.docid in (select docid from workflow.historicodocumento where aedid in (2023,2024)))";
        $obr = $this->pegaLinha($sql);
        return ($obr) ? true : false;
    }
    
    public function necessitaMarcaDaguaFNDE($arqid) {        
        //Não apagar os includes
        include_once APPRAIZ . 'www/obras2/_constantes.php';
        include_once APPRAIZ . 'www/obras2/_funcoes.php';
        include_once APPRAIZ . 'www/obras2/_componentes.php';
        include_once APPRAIZ . "www/autoload.php";
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        
        if($_SESSION['obras2']['semMarcadagua'] && possuiPerfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_EMPRESA_VISTORIADORA_GESTOR, PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC))){
            return false;
        }        
        $sql = "SELECT usucpf  FROM public.arquivo WHERE arqid = '" . $arqid . "'";
        $usucpf = $this->pegaUm($sql);
        if($usucpf == $_SESSION['usucpf']){
            return false;
        }else{
            if(possuiPerfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC))){
                return false;
            }else{
                if(possuiPerfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR, PFLCOD_EMPRESA_VISTORIADORA_FISCAL))){
                    return true;
                }
            }
            return true;
        }        
    }

    /**
     * Envia um email especifico quando a obra atinge um percentual
     * Para as obras PAC/TD envia um email quando atinge 30% e 60%
     * Para as obras de convênio envia um email quando atinge 25% e 50%
     *
     * @param null $obrid
     */
    public function enviaEmailEvolucao($obrid = null)
    {
        if($obrid)
            $this->__construct($obrid);
        $email = new Email();

        // PAC/TD else Convênio
        if ($this->tooid == 1 || $this->tooid == 11) {
            // Se tem mais de 30% envia o primeiro e-mail, a função enviaEmailPercentualExecucao faz o tratamento para enviar somente uma vez
            if($this->obrpercentultvistoria >= 30){
                $email->enviaEmailPercentualExecucao1($this->obrid);
            }
            // Se tem mais de 60% envia o primeiro e-mail, a função enviaEmailPercentualExecucao faz o tratamento para enviar somente uma vez
            if($this->obrpercentultvistoria >= 60){
                $email->enviaEmailPercentualExecucao2($this->obrid);
            }
        } else if ($this->tooid == 2) {
            // Se tem mais de 25% envia o primeiro e-mail, a função enviaEmailPercentualExecucao faz o tratamento para enviar somente uma vez
            if($this->obrpercentultvistoria >= 25){
                $email->enviaEmailPercentualExecucao1($this->obrid);
            }
            // Se tem mais de 50% envia o primeiro e-mail, a função enviaEmailPercentualExecucao faz o tratamento para enviar somente uma vez
            if($this->obrpercentultvistoria >= 50){
                $email->enviaEmailPercentualExecucao2($this->obrid);
            }
        }
    }
    
    
    public function verificaObraVinculada($obrid = NULL) {
        if(empty($obrid)){
            $obrid = $this->obrid;
            if(empty($obrid)){
                return false;
            }
        }else{
            $this->carregarPorId($obrid);
        }
        $obrid = (string)$obrid;
        if(substr($obrid, 0, 2) == '90' && $this->obridvinculado != ''){
            return true;
        }
        else{
            return false;
        }
    }

    public function pegaPrazoMI($obrid = null)
    {
        if($obrid)
            $this->carregarPorId($obrid);

        $prazo = false;

        switch ($this->tpoid){
            case 104: // tipo B
                $prazo = 210;
                break;
            case 105: // tipo C
                $prazo = 180;
                break;
        }

        return $prazo;
    }

    public function pegaPercentualExecucao($obrid)
    {
        $sql = "
                SELECT
                  ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2)
                FROM obras2.obras
                WHERE obrid = $obrid
        ";
        return $this->pegaUm($sql);
    }

    public function pegaPercentualExecucaoPactuado($obrid)
    {
        $sql = "
                SELECT
                  ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2)
                FROM obras2.obras
                WHERE obrid = $obrid
        ";
        $percentual =  $this->pegaUm($sql);

        $supervisao = new Supervisao();
        $supid = ($supid) ? $supid : $supervisao->pegaUltSupidByObra($obrid);

        $sqlPercNaoPactuado = "
                SELECT
                    SUM( spivlritemsobreobraexecanterior )
                FROM obras2.cronograma c
                JOIN obras2.itenscomposicaoobra ico ON ico.obrid = c.obrid AND ico.icostatus = 'A' AND c.croid = ico.croid AND ico.relativoedificacao = 'D'
                JOIN  obras2.itenscomposicao i ON i.itcid = ico.itcid AND i.itcstatus = 'A'
                JOIN obras2.supervisaoitem s ON s.icoid = ico.icoid AND s.supid = $supid
                WHERE c.obrid = $obrid AND c.crostatus = 'A' AND i.itcid IN (98)
        ";
        $percentualNaoPactuado =  $this->pegaUm($sqlPercNaoPactuado);

        return $percentual - $percentualNaoPactuado;
    }


    public function pegaPercentualExecucaoSupervisao($obrid, $supid)
    {
        $sql = "

                SELECT
                    ((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(SUM( spivlritemsobreobraexecanterior ),0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2)
                FROM obras2.cronograma c
                JOIN obras2.itenscomposicaoobra ico ON ico.obrid = c.obrid AND ico.icostatus = 'A' AND c.croid = ico.croid AND ico.relativoedificacao = 'D'
                JOIN obras2.itenscomposicao i ON i.itcid = ico.itcid AND i.itcstatus = 'A'
                JOIN obras2.supervisaoitem s ON s.icoid = ico.icoid AND s.supid = $supid
                JOIN obras2.obras o ON o.obrid = $obrid
                WHERE c.obrid = $obrid AND c.crostatus = 'A'
                GROUP BY o.obrperccontratoanterior

        ";
        return $this->pegaUm($sql);
    }

    public function pegaPercentualExecucaoPactuadoSupervisao($obrid, $supid)
    {
        $sql = "
                SELECT
                    SUM( spivlrfinanceiroanterior )
                FROM obras2.cronograma c
                JOIN obras2.itenscomposicaoobra ico ON ico.obrid = c.obrid AND ico.icostatus = 'A' AND c.croid = ico.croid  AND ico.relativoedificacao = 'D'
                JOIN obras2.itenscomposicao i ON i.itcid = ico.itcid AND i.itcstatus = 'A'
                JOIN obras2.supervisaoitem s ON s.icoid = ico.icoid AND s.supid = $supid
                JOIN obras2.obras o ON o.obrid = $obrid
                WHERE c.obrid = $obrid AND c.crostatus = 'A' AND i.itcid NOT IN (98)

        ";
        $valorExecutadoPactuado = $this->pegaUm($sql);

        $sql = "SELECT
                    SUM(ico.icovlritem) valortotalpactuado
                FROM obras2.itenscomposicaoobra  ico
                JOIN obras2.itenscomposicao  i ON i.itcid = ico.itcid
                JOIN obras2.cronograma c ON c.obrid = ico.obrid AND c.crostatus = 'A' AND c.croid = ico.croid
                WHERE
                    ico.icostatus = 'A' AND
                    ico.obrid = $obrid AND
                    ico.relativoedificacao = 'D' AND
                    ico.itcid NOT IN (98);";
        $valorObraPactuado = $this->pegaUm($sql);

        $percentExecutado = ($valorExecutadoPactuado * 100) / $valorObraPactuado;

        $sql = "
                SELECT
                  ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce($percentExecutado,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2)
                FROM obras2.obras
                WHERE obrid = $obrid
        ";
        $percentualExecutadoPactuado = $this->pegaUm($sql);
        $percentualExecutado = $this->pegaPercentualExecucaoSupervisao($obrid, $supid);

        return ($percentualExecutadoPactuado > $percentualExecutado) ? $percentualExecutado : $percentualExecutadoPactuado;
    }

    public function pegaUfObra($obrid)
    {
        $sql = "select estuf from obras2.obras obr
                join entidade.endereco
                 ende on ende.endid = obr.endid
                where obr.obrid = {$obrid} and obrstatus = 'A' ";
        return $this->pegaUm($sql);
    }



}