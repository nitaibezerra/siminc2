<?php

class EvolucaoMi extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.evolucaomi";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("emiid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'emiid'                => null,                                    
                                    'obrid'                => null,
                                    'docid'                => null,
                                    'usucpf'               => null,
                                    'emivalidador'         => null,
                                    'emidtevolucao'        => null,
                                    'emidtconclusaoprev'   => null,
                                    'emipecentmedido'      => null,
                                    'emivlmedido'          => null,
                                    'emiqtdmedido'         => null,
                                    'emipecentvalidado'    => null,
                                    'emivlvalidado'        => null,
                                    'emiqtdvalidado'       => null,
                                    'emipecenttotalmedido' => null,
                                    'emivltotalpago'       => null,
                                    'emidtinclusao'        => null,
                                    'emidtatualizacao'     => null,
                                    'emistatus'            => null
                                  );
    
    
    /**
     * 
     * @param int $obrid
     * @explicacao do uso da referência (&$obrid) http://www.php.net/manual/pt_BR/language.references.arent.php
     */
    public function validaParametroObrid(&$obrid) {
        if(empty($obrid) || !is_numeric($obrid)){
            $obrid = $this->obrid;
            if(empty($obrid)){
                $obrid = $_SESSION['obras2']['obrid'];
                if(empty($obrid)){
                    $obrid = false;
                }
            }
        }        
        $obrid = (int)$obrid;            
    }
    
    public function validaParametroEmiid(&$emiid) {
        if(empty($emiid) || !is_numeric($emiid)){
            $emiid = $this->emiid;
            if(empty($emiid)){
                $emiid = false;                
            }
        }
        $emiid = (int)$emiid;
    }
    
    public function acoesListaSql($emiid, $docid, $esdid, $simple = false) {
        
        switch ($esdid) {
            case ESDID_MI_CADASTRAMENTO:
                $acaoAlterar = "<img align=\"absmiddle\"
                                     src=\"/imagens/alterar.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: editarEvolucaoMi({$emiid});\"
                                     title=\"Alterar Evolução MI\" /> ";
                $acaoExcluir = "<img align=\"absmiddle\"
                                     src=\"/imagens/excluir.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: excluirEvolucaoMi({$emiid});\"
                                     title=\"Excluir Evolução MI\" /> ";
                $acaoImprimir = "<img align=\"absmiddle\"
                                     src=\"/imagens/print.png\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: imprimirEvolucaoMi({$emiid});\"
                                     title=\"Imprimir Evolução MI\" /> ";
                $acaoValidar = "<img align=\"absmiddle\"
                                     src=\"/imagens/valida1.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: alert('O estado atual da Evolução é \'Em Cadastramento\'. Para validar a Evolução MI, mude o estado para \'Validação\'.');\"
                                     title=\"Validar Evolução MI\" /> ";
                break;
            case ESDID_MI_VALIDACAO:
                $acaoAlterar = "<img align=\"absmiddle\"
                                    src=\"/imagens/alterar.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: editarEvolucaoMi({$emiid});\"
                                    title=\"Alterar Evolução MI\" /> ";
               $acaoExcluir = "<img align=\"absmiddle\"
                                    src=\"/imagens/excluir.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: excluirEvolucaoMi({$emiid});\"
                                    title=\"Excluir Evolução MI\" /> ";
               $acaoImprimir = "<img align=\"absmiddle\"
                                    src=\"/imagens/print.png\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: imprimirEvolucaoMi({$emiid});\"
                                    title=\"Imprimir Evolução MI\" /> ";
               $acaoValidar = "<img align=\"absmiddle\"
                                    src=\"/imagens/valida1.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: validarEvolucaoMi({$emiid});\"
                                    title=\"Validar Evolução MI\" /> ";
                break;
            case ESDID_MI_CONCLUIDO:
                $acaoAlterar = "<img align=\"absmiddle\"
                                    src=\"/imagens/alterar.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: editarEvolucaoMi({$emiid});\"
                                    title=\"Alterar Evolução MI\" /> ";
               $acaoExcluir = "<img align=\"absmiddle\"
                                    src=\"/imagens/excluir.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: alert('Evolução MI não pode ser excluída, pois já está concluída.');\"
                                    title=\"Excluir Evolução MI\" /> ";
               $acaoImprimir = "<img align=\"absmiddle\"
                                    src=\"/imagens/print.png\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: imprimirEvolucaoMi({$emiid});\"
                                    title=\"Imprimir Evolução MI\" /> ";
               $acaoValidar = "<img align=\"absmiddle\"
                                    src=\"/imagens/valida1.gif\"
                                    style=\"cursor: pointer\"
                                    onclick=\"javascript: alert('Evolução MI já foi Validada.');\"
                                    title=\"Validar Evolução MI\" /> ";
                break;
            case ESDID_MI_CORRECAO:
                $acaoAlterar = "<img align=\"absmiddle\"
                                     src=\"/imagens/alterar.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: editarEvolucaoMi({$emiid});\"
                                     title=\"Alterar Evolução MI\" /> ";
                $acaoExcluir = "<img align=\"absmiddle\"
                                     src=\"/imagens/excluir.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: excluirEvolucaoMi({$emiid});\"
                                     title=\"Excluir Evolução MI\" /> ";
                $acaoImprimir = "<img align=\"absmiddle\"
                                     src=\"/imagens/print.png\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: imprimirEvolucaoMi({$emiid});\"
                                     title=\"Imprimir Evolução MI\" /> ";
                $acaoValidar = "<img align=\"absmiddle\"
                                     src=\"/imagens/valida1.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: alert('O estado atual da Evolução é \'Correção\'. Para validar a Evolução MI, mude o estado para \'Validação\'.');\"
                                     title=\"Validar Evolução MI\" /> ";
                break;

            default: //ESDID_MI_CADASTRAMENTO
                
                if(empty($docid)){
                    $docid = $this->criarDocidEvolucaoMi($emiid);
                    $sql = 'UPDATE obras2.evolucaomi SET docid = '.$docid.' WHERE emiid = '.$emiid;
                    try{
                        $this->executar($sql);
                        $this->commit();
                    }catch (Exception $e){
                        $this->rollback();                        
                    }
                }
                
                $acaoAlterar = "<img align=\"absmiddle\"
                                     src=\"/imagens/alterar.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: editarEvolucaoMi({$emiid});\"
                                     title=\"Alterar Evolução MI\" /> ";
                $acaoExcluir = "<img align=\"absmiddle\"
                                     src=\"/imagens/excluir.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: excluirEvolucaoMi({$emiid});\"
                                     title=\"Excluir Evolução MI\" /> ";
                $acaoImprimir = "<img align=\"absmiddle\"
                                     src=\"/imagens/print.png\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: imprimirEvolucaoMi({$emiid});\"
                                     title=\"Imprimir Evolução MI\" /> ";
                $acaoValidar = "<img align=\"absmiddle\"
                                     src=\"/imagens/valida1.gif\"
                                     style=\"cursor: pointer\"
                                     onclick=\"javascript: alert('O estado atual da Evolução é \'Em Cadastramento\'. Para validar a Evolução MI, mude o estado para \'Validação\'.');\"
                                     title=\"Validar Evolução MI\" /> ";
                break;
        }

        if($simple){
            $acaoExcluir = $acaoValidar = '';
            if(!possui_perfil(array(
                    PFLCOD_EMPRESA_MI_GESTOR,
                    PFLCOD_EMPRESA_MI_FISCAL,
                    PFLCOD_GESTOR_UNIDADE,
                    PFLCOD_GESTOR_MEC,
                    PFLCOD_SUPERVISOR_UNIDADE,
                    PFLCOD_SUPER_USUARIO))){
                $acaoAlterar = '';
            }
        }

        $acoes = $acaoAlterar.' '.$acaoExcluir.' '.$acaoImprimir.' '.$acaoValidar;
        return $acoes;
    }
    
    public function listaEvolucaoCompletaSql(array $param = array(), $campos = '') {
        
        $where = '';
        
        if(!empty($param['obrid'])){
            $where = ' AND emi.obrid = '.$param['obrid'].' ';
        }else{
            $where = ' AND emi.obrid = '.$_SESSION['obras2']['obrid'].' ';
        }
        
        if(!empty($param['emiid'])){
            $where = ' AND emi.emiid = '.$param['emiid'].' ';
        }
        
        if(trim($campos) == ''){
            $campos = " emi.emiid,
                        emi.obrid,
                        emi.docid,
                        emi.usucpf,
                        su.usunome,
                        emi.emidtevolucao,
                        TO_CHAR(emi.emidtinclusao, 'YYYY-mm-dd') AS emidtinclusao,
                        -- Edificacao
                        emi.emipecentmedidoedif,
                        emi.emipecentvalidadoedif,
                        emi.emivlmedidoedif,
                        emi.emivlvalidadoedif,
                        -- Servicos externos
                        emi.emipecentmedidoext,
                        emi.emipecentvalidadoext,
                        emi.emivlmedidoext,                    
                        emi.emivlvalidadoext,
                        --Possui Foto
                        CASE WHEN (SELECT COUNT(emiid) FROM obras2.fotos fot WHERE fot.emiid = emi.emiid ) > 0 THEN
                            'Sim'
                        ELSE
                            'Não'
                        END as possuifoto,
                        d.*,
                        ed.*";
        }
        
        $sql = "  
                SELECT 
                    {$campos}
                        
                FROM obras2.evolucaomi as emi
                
                LEFT JOIN seguranca.usuario        su ON emi.usucpf = su.usucpf
                LEFT JOIN workflow.documento 	    d ON d.docid   = emi.docid --AND tpdid = 105
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                WHERE 
                    emistatus = 'A' {$where}
               ";

//                    ver($sql);
                    
        return $sql;
    }
    
    public function listaEvolucaoCompletaGeralSql(array $param = array(), $campos = '') {
        
        $where = array();
        
        $obrid = $param['obrbuscatexto'];
        if (!empty($obrid)) {
            $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrid))));
            $where[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                                        obr.obrid::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
        }
        
        if(!empty($param['emiid'])){
            $where[] = ' epmi.emiid = '.$param['emiid'].' ';
        }
        
        if(!empty($param['esdid'])){
            $where[] = ' ed.esdid = '.$param['esdid'].' ';
        }
        
        
        if(!empty($param['evo_foto'])){
            switch ($param['evo_foto']) {
                case 'S':
                    $where[] = " emi.emiid IN (SELECT emiid FROM obras2.fotos WHERE obrid IS NOT NULL AND emiid IS NOT NULL GROUP BY emiid) ";
                    break;
                case 'N':
                    $where[] = " emi.emiid NOT IN (SELECT emiid FROM obras2.fotos WHERE obrid IS NOT NULL AND emiid IS NOT NULL GROUP BY emiid) ";
                    break;
                default:
                    break;
            }
        }
        
        $mun = $param['muncod'];
        if (!empty($mun)) {
            if (is_array($mun)) {
                foreach ($mun as $k => $v) {
                    $mun[$k] = "'{$v}'";
                }
            } else {
                $mun = array("'{$mun}'");
            }
            foreach ($mun as $k => $v) {
                if (trim($v) == '' || $v == "''" || empty($v)) {
                    unset($mun[$k]);
                }
            }
            if (!empty($mun)) {
                $where[] = " mun.muncod IN ( " . implode(",", $mun) . " ) ";
            }
        }

        $uf = $param['estuf'];
        if (!empty($uf)) {
            if (is_array($uf)) {
                foreach ($uf as $k => $v) {
                    $uf[$k] = "'{$v}'";
                }
            } else {
                $uf = array("'{$uf}'");
            }
            foreach ($uf as $k => $v) {
                if (trim($v) == '' || $v == "''" || empty($v)) {
                    unset($uf[$k]);
                }
            }
            if (!empty($uf)) {
                $where[] = " mun.estuf IN ( " . implode(",", $uf) . " ) ";
            }
        }
        
        if (possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE,PFLCOD_CONSULTA_UNIDADE)) && !$this->testa_superuser()) {
            
            $usuarioResp = new UsuarioResponsabilidade();
            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
            $arEmpid = ($arEmpid ? $arEmpid : array(0));

            $resp[] = " urs.entid = e.entidunidade AND e.empid IN('" . implode("', '", $arEmpid) . "')";
            $arPflcod[] = PFLCOD_GESTOR_UNIDADE;
            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;
            
            if (count($resp)) {
                $arPflcod = array_unique($arPflcod);
                $usuarioresponsabilidade = "JOIN obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A' AND
                                                                                       urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                                       urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                                       (" . implode(' OR ', $resp) . ")";
            }
            
            
        }else{
            $usuarioresponsabilidade = '';
        }
        
        if (possui_perfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_ADMINISTRATIVO)) && !$this->testa_superuser()) {

            $joinEmpresaMI = " 	INNER JOIN obras2.empresami_uf euf ON euf.estuf = ende.estuf AND euf.eufstatus = 'A'
                                INNER JOIN obras2.empresami   epmi ON epmi.emiid = euf.emiid  AND epmi.emistatus = 'A'";
            $where[] = "
                        epmi.emiid IN (SELECT urs.emiid FROM obras2.usuarioresponsabilidade urs
                                            WHERE  urs.rpustatus = 'A' AND urs.usucpf = '{$_SESSION['usucpf']}'  
                                            GROUP BY urs.emiid)
                       ";
        }  else {
            $joinEmpresaMI = "LEFT JOIN obras2.empresami_uf euf ON euf.estuf  = ende.estuf AND euf.eufstatus = 'A'
                              LEFT JOIN obras2.empresami   epmi ON epmi.emiid = euf.emiid AND epmi.emistatus = 'A'";
        }
        
        $where = !empty($where) ? ' AND ' . implode(' AND ', $where) : '';
        
        if($_REQUEST['tipo_relatorio'] == 'xls'){
            $select_obrid = " '(' || obr.obrid || ') ' || obr.obrnome, ";
        }else{
            $select_obrid = " '<a href=\"obras2.php?modulo=principal/listaEvolucaoMi&acao=A&obrid=' || obr.obrid || '\">(' || obr.obrid || ') ' || obr.obrnome || '</a>'  as nome_obra, -- * Nome da Obra ";
            
        }
        
        if(trim($campos) == ''){
            $campos = " --emi.emiid,
                        {$select_obrid}
                        mun.estuf as estado, -- * Estado
                        mun.mundescricao as municipio, -- * MINICIPIO
                        TO_CHAR(emi.emidtevolucao, 'dd/mm/YYYY') AS emidtevolucao,
                        TO_CHAR(emi.emidtinclusao, 'dd/mm/YYYY') AS emidtinclusao,
                        su.usunome,
                        -- Edificacao
                        emi.emipecentmedidoedif,
                        emi.emipecentvalidadoedif,
                        emi.emivlmedidoedif,
                        emi.emivlvalidadoedif,
                        -- Servicos externos
                        emi.emipecentmedidoext,
                        emi.emipecentvalidadoext,
                        emi.emivlmedidoext,
                        emi.emivlvalidadoext,
                        --Possui Foto
                        CASE WHEN (SELECT COUNT(emiid) FROM obras2.fotos fot WHERE fot.obrid = obr.obrid AND fot.emiid IS NOT NULL ) > 0 THEN
                            'Sim'
                        ELSE
                            'Não'
                        END as possuifoto,

                        --Situação
                        ed.esddsc,
                        --Última tramitação
                        usu_acao.usunome as usu_nome_exec_tramitacao";
        }
        
        $sql = "  
                SELECT 
                    {$campos}
                        
                FROM obras2.evolucaomi as emi
                
                LEFT  JOIN seguranca.usuario           su ON emi.usucpf  = su.usucpf
                LEFT  JOIN workflow.documento 	        d ON d.docid     = emi.docid
                LEFT  JOIN workflow.estadodocumento    ed ON ed.esdid    = d.esdid
                LEFT  JOIN workflow.historicodocumento hst ON hst.hstid  = d.hstid
                LEFT  JOIN seguranca.usuario      usu_acao ON hst.usucpf = usu_acao.usucpf 
                INNER JOIN obras2.obras                obr ON emi.obrid  = obr.obrid    
                INNER JOIN obras2.empreendimento         e ON e.empid    = obr.empid
                LEFT  JOIN entidade.endereco          ende ON ende.endid = obr.endid AND ende.endstatus = 'A'
                LEFT  JOIN territorios.municipio       mun ON mun.muncod = ende.muncod
                LEFT  JOIN territorios.estado           uf ON mun.estuf  = uf.estuf

                {$joinEmpresaMI}
                {$usuarioresponsabilidade}

                WHERE 
                    emi.emistatus = 'A' {$where}
               ";

//                    ver($sql);
        return $sql;
    }
    
    public function listaArray(array $param = array(), $simple = false) {
        $sql = $this->listaEvolucaoCompletaSql($param);

        $arr = $this->carregar($sql);
        $arr = (!empty($arr)) ? $arr : array();
        
        foreach ($arr as $key => $value) {
            $arr_tramit            = $this->getDadosTramitacaoEvolucaoMi($value['docid']);
            $arr[$key]['acao']     = $this->acoesListaSql($value['emiid'], $value['docid'], $value['esdid'], $simple);
            $arr[$key]['situacao'] = $arr_tramit['esddsc'];
            
            if($value['esdid'] == ESDID_MI_CONCLUIDO){
                
                $data = explode('-', $arr_tramit['data_tramitacao2']);
                $data = $data[2].'/'.$data[1].'/'.$data[0];

                $ultima_tramitacao = ''.$arr_tramit['usu_nome_exec_tramitacao'].'<br><i>( '.$data.' )</i>';
                $arr[$key]['ultima_tramitacao'] = $ultima_tramitacao;
                
            }
            else{
                if(empty($arr_tramit['usu_nome_exec_tramitacao'])){
                    $data1    = $value['emidtinclusao'];
                    $data2    = date('Y-m-d');
                    $diff_dts = retornaDiasEntreDatas($data1, $data2);

                    if($diff_dts > 5){
                        $diff_dts = '<span style="color: red"> '.$diff_dts.' dias </span>';                    
                    }elseif($diff_dts == 0){
                        $diff_dts = '<span style="color: black"> '.$diff_dts.' dia </span>';
                    }else{
                        $diff_dts = '<span style="color: black"> '.$diff_dts.' dias </span>';
                    }

                    $data = explode('-', $value['emidtinclusao']);
                    $data = $data[2].'/'.$data[1].'/'.$data[0];

                    $ultima_tramitacao = ''.$value['usunome'].'<br><i>('.$data.' - '.$diff_dts.')</i>';
                    $arr[$key]['ultima_tramitacao'] = $ultima_tramitacao;
                }
                else{

                    $data1    = $arr_tramit['data_tramitacao2'];
                    $data2    = date('Y-m-d');
                    $diff_dts = retornaDiasEntreDatas($data1, $data2);

                    if($diff_dts > 5){
                        $diff_dts = '<span style="color: red"> '.$diff_dts.' dias </span>';                    
                    }elseif($diff_dts == 0){
                        $diff_dts = '<span style="color: black"> '.$diff_dts.' dia </span>';
                    }else{
                        $diff_dts = '<span style="color: black"> '.$diff_dts.' dias </span>';
                    }

                    $data = explode('-', $arr_tramit['data_tramitacao2']);
                    $data = $data[2].'/'.$data[1].'/'.$data[0];

                    $ultima_tramitacao = ''.$arr_tramit['usu_nome_exec_tramitacao'].'<br><i>('.$data.' - '.$diff_dts.')</i>';
                    $arr[$key]['ultima_tramitacao'] = $ultima_tramitacao;                
                }
                
            }
        }
        
//        ver($arr);
        
        return $arr;
    }
    
    public function getUltEmiidByObrid($obrid) {
        $this->validaParametroObrid($obrid);
        if(empty($obrid)){ return false; }
        
        $sql = "SELECT
                        MAX(emiid) AS emiid
                FROM
                        obras2.evolucaomi
                WHERE
                        emistatus = 'A' AND
                        obrid = {$obrid}";

        return $this->pegaUm($sql);
    }
    
    public function getDocidEmiid($emiid) {
        $this->validaParametroEmiid($emiid);
        if(empty($emiid)){ return false; }
        
        $sql = "SELECT
                        docid
                FROM
                        obras2.evolucaomi
                WHERE
                        emistatus = 'A' AND
                        emiid = {$emiid}";

        return $this->pegaUm($sql);
    }
    
    public function getDadosEvolucaoMi($emiid, $arr_campos = array()) {
        $this->validaParametroEmiid($emiid);
        if(empty($emiid)){ return array(); }
        if(empty($arr_campos)){
            $campos = 'emi.*, d.*, ed.*, o.empid';
        }else{
            $campos = implode(', ', $arr_campos);
        }
        $sql = 'SELECT '.$campos.'  
                FROM obras2.evolucaomi emi
                INNER JOIN obras2.obras             o ON emi.obrid = o.obrid
                LEFT JOIN seguranca.usuario        su ON emi.usucpf = su.usucpf
                LEFT JOIN workflow.documento 	    d ON d.docid   = emi.docid --AND tpdid = 105
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                WHERE emi.emiid = '.$emiid;
        $arr_dados = $this->pegaLinha($sql);
        $arr_dados = ($arr_dados) ? $arr_dados : array();
        return $arr_dados;
    }
    
    public function gravarDadosEvoluçãoMiParaFotos() {
        
        if(!empty($_POST['emidtevolucao'])){
            $emidtevolucao = explode('/', $_POST['emidtevolucao']);
            $emidtevolucao = "'".$emidtevolucao[2].'-'.$emidtevolucao[1].'-'.$emidtevolucao[0]."'";
        }else{
            $emidtevolucao = 'NOW()';
        }
        
        if(!empty($_POST['emidtconclusaoprev'])){
            $emidtconclusaoprev = explode('/', $_POST['emidtconclusaoprev']);
            $emidtconclusaoprev = "'".$emidtconclusaoprev[2].'-'.$emidtconclusaoprev[1].'-'.$emidtconclusaoprev[0]."'";
        }else{
            $emidtconclusaoprev = 'NOW()';
        }
                
        
        $dados_evolucao['obrid']                = $_SESSION['obras2']['obrid'];
        $dados_evolucao['docid']                = 'NULL';
        $dados_evolucao['usucpf']               = "'".$_SESSION['usucpf']."'";
        $dados_evolucao['emivalidador']         = 'NULL';
        $dados_evolucao['emidtevolucao']        = $emidtevolucao;
        $dados_evolucao['emidtconclusaoprev']   = $emidtconclusaoprev;
        $dados_evolucao['emipecentmedidoedif']  = 0.00;
        $dados_evolucao['emivlmedidoedif']      = 0.00;
        $dados_evolucao['emipecentmedidoext']   = 0.00;
        $dados_evolucao['emivlmedidoext']       = 0.00;
        $dados_evolucao['emipecenttotalmedido'] = 0;
        $dados_evolucao['emivltotalpago']       = 0;
        $dados_evolucao['emidtinclusao']        = 'now()';
        $dados_evolucao['emidtatualizacao']     = 'now()';
        $dados_evolucao['emistatus']            = "'A'";
        
        $insert_evolucao_campos  = "";
        $insert_evolucao_valores = "";        
        foreach ($dados_evolucao as $key => $value) {
            $insert_evolucao_campos  .= $key.",";
            $insert_evolucao_valores .= $value.",";
        }
        $insert_evolucao = " INSERT INTO obras2.evolucaomi (".substr($insert_evolucao_campos,0,-1).") VALUES (".substr($insert_evolucao_valores,0,-1).") RETURNING emiid; ";

        try{
            $emiid = $this->pegaUm($insert_evolucao);
            $this->emiid = $emiid;
        } catch (Exception $ex) {
            $this->rollback();
            $arr_return['erro'] = true;
            $arr_return['msg'] .= '\nErro ao cadastrar a Evolução MI. '.$ex->getMessage();
        }

        $docid = $this->atualizarDocidEvolucaoMi($emiid);
        
        if(!empty($emiid)){
            $dados_item_d_evolucao = array();
            $_POST['icoid_d'] = (is_array($_POST['icoid_d']) && !empty($_POST['icoid_d'])) ? $_POST['icoid_d'] : array();
            foreach ($_POST['icoid_d'] as $key => $value) {
                $dados_item_d_evolucao[$key]['emiid']                  = $emiid;
                $dados_item_d_evolucao[$key]['icoid']                  = $value;
                $dados_item_d_evolucao[$key]['iemipecentmedido']       = str_replace(',', '.',(str_replace('.', '',$_POST['iemipecentmedido'][$key])));
                $dados_item_d_evolucao[$key]['iemivlmedido']           = str_replace(',', '.',(str_replace('.', '',$_POST['iemivlmedido'][$key])));
                $dados_item_d_evolucao[$key]['iemiqtdmedido']          = 0;
                $dados_item_d_evolucao[$key]['iemipecentvalidado']     = 0;
                $dados_item_d_evolucao[$key]['iemivlvalidado']         = 0;
                $dados_item_d_evolucao[$key]['iemiqtdvalidado']        = 0;
                $dados_item_d_evolucao[$key]['iemiqtditem']            = 0;
                $dados_item_d_evolucao[$key]['usucpf']                 = "'".$_SESSION['usucpf']."'";
                $dados_item_d_evolucao[$key]['iemidtinclusao']         = 'now()';
                $dados_item_d_evolucao[$key]['iemidtatualizacao']      = 'now()';
                $dados_item_d_evolucao[$key]['iemistatus']             = "'A'";
                $dados_item_d_evolucao[$key]['iemirelativoedificacao'] = "'D'";
            }

            $dados_item_f_evolucao = array();
            $_POST['icoid_f'] = (empty($_POST['icoid_f']) || !is_array($_POST['icoid_f'])) ? array() : $_POST['icoid_f'];
            foreach ($_POST['icoid_f'] as $key => $value) {
                $dados_item_f_evolucao[$key]['emiid']                  = $emiid;
                $dados_item_f_evolucao[$key]['icoid']                  = $value;
                $dados_item_f_evolucao[$key]['iemipecentmedido']       = (!empty($_POST['iemipecentmedido_f'][$key]) ? str_replace(',', '.',(str_replace('.', '',$_POST['iemipecentmedido_f'][$key]))) : 0);
                $dados_item_f_evolucao[$key]['iemivlmedido']           = (!empty($_POST['iemivlmedido_f'][$key]) ? str_replace(',', '.',(str_replace('.', '',$_POST['iemivlmedido_f'][$key]))) : 0);
                $dados_item_f_evolucao[$key]['iemiqtdmedido']          = str_replace(',', '.',(str_replace('.', '',$_POST['iemiqtdmedido'][$key])));
                $dados_item_f_evolucao[$key]['iemipecentvalidado']     = 0;
                $dados_item_f_evolucao[$key]['iemivlvalidado']         = 0;
                $dados_item_f_evolucao[$key]['iemiqtdvalidado']        = 0;
                $dados_item_f_evolucao[$key]['iemiqtditem']            = (!empty($_POST['itcquantidade'])) ? str_replace(',', '.',(str_replace('.', '',$_POST['itcquantidade'][$key]))) : 0;
                $dados_item_f_evolucao[$key]['usucpf']                 = "'".$_SESSION['usucpf']."'";
                $dados_item_f_evolucao[$key]['iemidtinclusao']         = 'now()';
                $dados_item_f_evolucao[$key]['iemidtatualizacao']      = 'now()';
                $dados_item_f_evolucao[$key]['iemistatus']             = "'A'";
                $dados_item_f_evolucao[$key]['iemirelativoedificacao'] = "'F'";
            }

            foreach ($dados_item_d_evolucao as $key => $item) {
                $insert_item_d_campos  = "";
                $insert_item_d_valores = "";        
                $insert_item_d = "";
                foreach ($item as $k => $v) {
                    $insert_item_d_campos  .= $k.",";
                    $insert_item_d_valores .= $v.",";
                }            
                $insert_item_d = " INSERT INTO obras2.itemevolucaomi(".substr($insert_item_d_campos,0,-1).") VALUES (".substr($insert_item_d_valores,0,-1).") RETURNING iemiid; ";
                try{
                    $iemiid = $this->pegaUm($insert_item_d);
                } catch (Exception $ex) {
                    $this->rollback();
                    $arr_return['erro'] = true;
                    $arr_return['msg'] .= '\nErro ao cadastrar a os itens do cronograma da Evolução MI. '.$ex->getMessage();
                }
            }

            foreach ($dados_item_f_evolucao as $key => $item) {
                $insert_item_f_campos  = "";
                $insert_item_f_valores = "";        
                $insert_item_f = "";
                foreach ($item as $k => $v) {
                    $insert_item_f_campos  .= $k.",";
                    $insert_item_f_valores .= $v.",";
                }
                $insert_item_f = " INSERT INTO obras2.itemevolucaomi(".substr($insert_item_f_campos,0,-1).") VALUES (".substr($insert_item_f_valores,0,-1).") RETURNING iemiid; ";

                try{
                    $iemiid = $this->pegaUm($insert_item_f);
                } catch (Exception $ex) {
                    $this->rollback();
                    $arr_return['erro'] = true;
                    $arr_return['msg'] .= '\nErro ao cadastrar a os itens de serviços externos da Evolução MI. '.$ex->getMessage();
                }
            }
        }
        
        if($arr_return['erro'] !== true){
            $this->commit();
        }
        
        $arr_return['emiid'] = $emiid;
        $arr_return['docid'] = $docid;
        
        return $arr_return;
        
    }
    
    public function gravarDadosEvoluçãoMi($tipo = 'salvar') {
        /**
         * 
         * $tipo = 'salvar'/'atualizar'/'validar'
         * 
         * Passo-a-passo
         * 
         * 01 - Recupera o array de dados dos itens da evolução, tipo D - OK
         * 02 - Recupera o array de dados dos itens da evolução, tipo F - OK
         * 03 - Calcula os valores totais dos itens D e F - OK
         * 
         * 04 - Monta array de dados da evolução - OK
         * 05 - Monta array de dados dos itens da evolução, tipo D - OK
         * 06 - Monta array de dados dos itens da evolução, tipo F - OK
         * 
         * 07 - Faz os cálculos (tendo como base a coluna de dinheiro) entre os arrays dos itens 1 e 5 (tipo D) - OK
         * 08 - Faz os cálculos (tendo como base a coluna de dinheiro) entre os arrays dos itens 2 e 6 (tipo F) - OK
         * 
         * 09 - Monta as querys de INSERT/UPDATE dos dados da evolução                   - OK
         * 10 - Monta as querys de INSERT/UPDATE dos dados dos itens da evolução, tipo D - OK
         * 11 - Monta as querys de INSERT/UPDATE dos dados dos itens da evolução, tipo F - OK
         * 
         * 
         * Cálculos:
         * 1 - Calcula o percentual executado do item de acordo com o valor da execução e o valor original do item  - OK
         * 2 - Soma os valores da execução                                                                          - OK
         * 3 - Calcula o percentual da evolução de acordo com o valor executado e o valor total                     - OK
         * 4 - Compara os valores de dinheiro e percentual da evolução e ajusta caso necessário                     - OK
         * 
         */
        
        $arr_return = array('emiid' => 0, 'docid' => 0, 'erro'  => false, 'msg'   => '');
        
        $obrid = $_SESSION['obras2']['obrid'];
        //01
        $arr_dados_d = $this->getItemComposicaoObraByObra($obrid, 'D');
        //02
        $arr_dados_f = $this->getItemComposicaoObraByObra($obrid, 'F');
        
        $vl_total_d = 0;
        $vl_total_f = 0;
        
        $sum_valores_evo_d = 0;
        $sum_valores_evo_f = 0;
        //03
        foreach ($arr_dados_d as $key => $value) {
            $vl_total_d += $value['icovlritem'];
        }
        //03
        foreach ($arr_dados_f as $key => $value) {
            $vl_total_f += $value['icovlritem']*$value['itcquantidade'];
        }
        
        //04,05,06
        //Monta os arrays
        switch ($tipo) {
            case 'salvar':
                //04
                $emidtevolucao      = explode('/', $_POST['emidtevolucao']);
                $emidtevolucao      = "'".$emidtevolucao[2].'-'.$emidtevolucao[1].'-'.$emidtevolucao[0]."'";
                $emidtconclusaoprev = explode('/', $_POST['emidtconclusaoprev']);
                $emidtconclusaoprev = "'".$emidtconclusaoprev[2].'-'.$emidtconclusaoprev[1].'-'.$emidtconclusaoprev[0]."'";

                $dados_evolucao['obrid']                = $_SESSION['obras2']['obrid'];
                $dados_evolucao['docid']                = 'NULL';
                $dados_evolucao['usucpf']               = "'".$_SESSION['usucpf']."'";
                $dados_evolucao['emivalidador']         = 'NULL';
                $dados_evolucao['emidtevolucao']        = $emidtevolucao;
                $dados_evolucao['emidtconclusaoprev']   = $emidtconclusaoprev;
                $dados_evolucao['emipecentmedidoedif']  = str_replace(',', '.',(str_replace('.', '',$_POST['emipecentmedidoedif'])));
                $dados_evolucao['emivlmedidoedif']      = str_replace(',', '.',(str_replace('.', '',$_POST['emivlmedidoedif'])));
                $dados_evolucao['emipecentmedidoext']   = str_replace(',', '.',(str_replace('.', '',$_POST['emipecentmedidoext'])));
                $dados_evolucao['emivlmedidoext']       = str_replace(',', '.',(str_replace('.', '',$_POST['emivlmedidoext'])));
                $dados_evolucao['emipecenttotalmedido'] = 0;
                $dados_evolucao['emivltotalpago']       = 0;
                $dados_evolucao['emidtinclusao']        = 'now()';
                $dados_evolucao['emidtatualizacao']     = 'now()';
                $dados_evolucao['emistatus']            = "'A'";
                
                //05
                $dados_item_d_evolucao = array();
                $_POST['icoid_d'] = (is_array($_POST['icoid_d']) && !empty($_POST['icoid_d'])) ? $_POST['icoid_d'] : array();
                foreach ($_POST['icoid_d'] as $key => $value) {
                    $dados_item_d_evolucao[$key]['emiid']                  = 0;
                    $dados_item_d_evolucao[$key]['icoid']                  = $value;
                    $dados_item_d_evolucao[$key]['iemipecentmedido']       = str_replace(',', '.',(str_replace('.', '',$_POST['iemipecentmedido'][$key])));
                    $dados_item_d_evolucao[$key]['iemivlmedido']           = str_replace(',', '.',(str_replace('.', '',$_POST['iemivlmedido'][$key])));
                    $dados_item_d_evolucao[$key]['iemiqtdmedido']          = 0;
                    $dados_item_d_evolucao[$key]['iemipecentvalidado']     = 0;
                    $dados_item_d_evolucao[$key]['iemivlvalidado']         = 0;
                    $dados_item_d_evolucao[$key]['iemiqtdvalidado']        = 0;
                    $dados_item_d_evolucao[$key]['iemiqtditem']            = 0;
                    $dados_item_d_evolucao[$key]['usucpf']                 = "'".$_SESSION['usucpf']."'";
                    $dados_item_d_evolucao[$key]['iemidtinclusao']         = 'now()';
                    $dados_item_d_evolucao[$key]['iemidtatualizacao']      = 'now()';
                    $dados_item_d_evolucao[$key]['iemistatus']             = "'A'";
                    $dados_item_d_evolucao[$key]['iemirelativoedificacao'] = "'D'";
                }
                
                //06
                $dados_item_f_evolucao = array();
                $_POST['icoid_f'] = (empty($_POST['icoid_f']) || !is_array($_POST['icoid_f'])) ? array() : $_POST['icoid_f'];
                foreach ($_POST['icoid_f'] as $key => $value) {
                    $dados_item_f_evolucao[$key]['emiid']                  = 0;
                    $dados_item_f_evolucao[$key]['icoid']                  = $value;
                    $dados_item_f_evolucao[$key]['iemipecentmedido']       = (!empty($_POST['iemipecentmedido_f'][$key]) ? str_replace(',', '.',(str_replace('.', '',$_POST['iemipecentmedido_f'][$key]))) : 0);
                    $dados_item_f_evolucao[$key]['iemivlmedido']           = (!empty($_POST['iemivlmedido_f'][$key]) ? str_replace(',', '.',(str_replace('.', '',$_POST['iemivlmedido_f'][$key]))) : 0);
                    $dados_item_f_evolucao[$key]['iemiqtdmedido']          = str_replace(',', '.',(str_replace('.', '',$_POST['iemiqtdmedido'][$key])));
                    $dados_item_f_evolucao[$key]['iemipecentvalidado']     = 0;
                    $dados_item_f_evolucao[$key]['iemivlvalidado']         = 0;
                    $dados_item_f_evolucao[$key]['iemiqtdvalidado']        = 0;
                    $dados_item_f_evolucao[$key]['iemiqtditem']            = (!empty($_POST['itcquantidade'])) ? str_replace(',', '.',(str_replace('.', '',$_POST['itcquantidade'][$key]))) : 0;
                    $dados_item_f_evolucao[$key]['usucpf']                 = "'".$_SESSION['usucpf']."'";
                    $dados_item_f_evolucao[$key]['iemidtinclusao']         = 'now()';
                    $dados_item_f_evolucao[$key]['iemidtatualizacao']      = 'now()';
                    $dados_item_f_evolucao[$key]['iemistatus']             = "'A'";
                    $dados_item_f_evolucao[$key]['iemirelativoedificacao'] = "'F'";
                }

                break;
            case 'atualizar':
                $emiid              = $_POST['emiid'];
                $emidtevolucao      = explode('/', $_POST['emidtevolucao']);
                $emidtevolucao      = "'".$emidtevolucao[2].'-'.$emidtevolucao[1].'-'.$emidtevolucao[0]."'";
                $emidtconclusaoprev = explode('/', $_POST['emidtconclusaoprev']);
                $emidtconclusaoprev = "'".$emidtconclusaoprev[2].'-'.$emidtconclusaoprev[1].'-'.$emidtconclusaoprev[0]."'";

                $dados_evolucao['usucpf']              = "'".$_SESSION['usucpf']."'";
                $dados_evolucao['emidtevolucao']       = $emidtevolucao;
                $dados_evolucao['emidtconclusaoprev']  = $emidtconclusaoprev;        
                $dados_evolucao['emipecentmedidoedif'] = str_replace(',', '.',(str_replace('.', '',$_POST['emipecentmedidoedif'])));
                $dados_evolucao['emivlmedidoedif']     = str_replace(',', '.',(str_replace('.', '',$_POST['emivlmedidoedif'])));
                $dados_evolucao['emipecentmedidoext']  = str_replace(',', '.',(str_replace('.', '',$_POST['emipecentmedidoext'])));
                $dados_evolucao['emivlmedidoext']      = str_replace(',', '.',(str_replace('.', '',$_POST['emivlmedidoext'])));        
                $dados_evolucao['emidtatualizacao']    = 'now()';
                $dados_evolucao['emistatus']           = "'A'";
                
                $dados_item_d_evolucao = array();
                $_POST['icoid_d']      = (is_array($_POST['icoid_d']) && !empty($_POST['icoid_d'])) ? $_POST['icoid_d'] : array();
                foreach ($_POST['icoid_d'] as $key => $value) {
                    $dados_item_d_evolucao[$key]['iemipecentmedido']  = str_replace(',', '.',(str_replace('.', '',$_POST['iemipecentmedido'][$key])));
                    $dados_item_d_evolucao[$key]['iemivlmedido']      = str_replace(',', '.',(str_replace('.', '',$_POST['iemivlmedido'][$key])));
                    $dados_item_d_evolucao[$key]['usucpf']            = "'".$_SESSION['usucpf']."'";
                    $dados_item_d_evolucao[$key]['iemidtatualizacao'] = 'now()';            
                    $dados_item_d_evolucao[$key]['iemiid']            = $_POST['iemiid_d'][$key];
                }
                
                $dados_item_f_evolucao = array();
                $_POST['icoid_f']      = (empty($_POST['icoid_f']) || !is_array($_POST['icoid_f'])) ? array() : $_POST['icoid_f'];
                foreach ($_POST['icoid_f'] as $key => $value) {
                    $dados_item_f_evolucao[$key]['iemipecentmedido']  = (!empty($_POST['iemipecentmedido_f'][$key]) ? str_replace(',', '.',(str_replace('.', '',$_POST['iemipecentmedido_f'][$key]))) : 0);
                    $dados_item_f_evolucao[$key]['iemivlmedido']      = (!empty($_POST['iemivlmedido_f'][$key]) ? str_replace(',', '.',(str_replace('.', '',$_POST['iemivlmedido_f'][$key]))) : 0);
                    $dados_item_f_evolucao[$key]['iemiqtdmedido']     = str_replace(',', '.',(str_replace('.', '',$_POST['iemiqtdmedido'][$key])));
                    $dados_item_f_evolucao[$key]['usucpf']            = "'".$_SESSION['usucpf']."'";
                    $dados_item_f_evolucao[$key]['iemidtatualizacao'] = 'now()';
                    $dados_item_f_evolucao[$key]['iemiid']            = $_POST['iemiid_f'][$key];
                }

                break;
            case 'validar':
                $emiid              = $_POST['emiid'];                
                $emidtevolucao      = explode('/', $_POST['emidtevolucao']);
                $emidtevolucao      = "'".$emidtevolucao[2].'-'.$emidtevolucao[1].'-'.$emidtevolucao[0]."'";
                $emidtconclusaoprev = explode('/', $_POST['emidtconclusaoprev']);
                $emidtconclusaoprev = "'".$emidtconclusaoprev[2].'-'.$emidtconclusaoprev[1].'-'.$emidtconclusaoprev[0]."'";

                $dados_evolucao['emivalidador']          = "'".$_SESSION['usucpf']."'";
                $dados_evolucao['emidtevolucao']         = $emidtevolucao;
                $dados_evolucao['emidtconclusaoprev']    = $emidtconclusaoprev;
                $dados_evolucao['emipecentvalidadoedif'] = str_replace(',', '.',(str_replace('.', '',$_POST['emipecentvalidadoedif'])));
                $dados_evolucao['emivlvalidadoedif']     = str_replace(',', '.',(str_replace('.', '',$_POST['emivlvalidadoedif'])));
                $dados_evolucao['emipecentvalidadoext']  = str_replace(',', '.',(str_replace('.', '',$_POST['emipecentvalidadoext'])));
                $dados_evolucao['emivlvalidadoext']      = str_replace(',', '.',(str_replace('.', '',$_POST['emivlvalidadoext'])));
                $valor_total_pago                        = $dados_evolucao['emivlvalidadoedif'] + $dados_evolucao['emivlvalidadoext'];
                $dados_evolucao['emivltotalpago']        = $valor_total_pago;
                $dados_evolucao['emidtatualizacao']      = 'now()';
                $dados_evolucao['emistatus']             = "'A'";
                
                $dados_item_d_evolucao = array();
                $_POST['icoid_d']      = (empty($_POST['icoid_d'])) ? array() : $_POST['icoid_d'];
                foreach ($_POST['icoid_d'] as $key => $value){
                    $dados_item_d_evolucao[$key]['iemipecentvalidado'] = str_replace(',', '.',(str_replace('.', '',$_POST['iemipecentmedido_val'][$key])));
                    $dados_item_d_evolucao[$key]['iemivlvalidado']     = str_replace(',', '.',(str_replace('.', '',$_POST['iemivlmedido_val'][$key])));
                    $dados_item_d_evolucao[$key]['usucpf']             = "'".$_SESSION['usucpf']."'";
                    $dados_item_d_evolucao[$key]['iemidtatualizacao']  = 'now()';            
                    $dados_item_d_evolucao[$key]['iemiid']             = $_POST['iemiid_d'][$key];
                }
                
                $dados_item_f_evolucao = array();
                $_POST['icoid_f']      = (empty($_POST['icoid_f'])) ? array() : $_POST['icoid_f'];
                foreach ($_POST['icoid_f'] as $key => $value){            
                    $dados_item_f_evolucao[$key]['iemipecentvalidado'] = (!empty($_POST['iemipecentvalidado_f'][$key]) ? str_replace(',', '.',(str_replace('.', '',$_POST['iemipecentvalidado_f'][$key]))) : 0);
                    $dados_item_f_evolucao[$key]['iemivlvalidado']     = (!empty($_POST['iemivlvalidado_f'][$key]) ? str_replace(',', '.',(str_replace('.', '',$_POST['iemivlvalidado_f'][$key]))) : 0);
                    $dados_item_f_evolucao[$key]['iemiqtdvalidado']    = str_replace(',', '.',(str_replace('.', '',$_POST['iemiqtdvalidado_f'][$key])));
                    $dados_item_f_evolucao[$key]['usucpf']             = "'".$_SESSION['usucpf']."'";
                    $dados_item_f_evolucao[$key]['iemidtatualizacao']  = 'now()';
                    $dados_item_f_evolucao[$key]['iemiid']             = $_POST['iemiid_f'][$key];
                }

                break;
            default:
                break;
        }
        
        //07,08
        //Faz os cálculos
        if($tipo == 'validar') {
            //07
            foreach ($dados_item_d_evolucao as $key => $value) {
                foreach ($arr_dados_d as $k => $v) {                        
                    if($v['icoid'] == $value['icoid']){                            
                        $vl_total_item = $v['dadosCalculados']['vlTotalItem'];
                        $vl_item_evo   = $value['iemivlvalidado'];
                        //* 1 - Calcula o percentual executado do item de acordo com o valor da execução e o valor original do item
                        $percent_item = (!empty($vl_total_item) && $vl_total_item != '0.00' ) ? ($vl_item_evo*100)/$vl_total_item : 0;
                        $vl_item_evo = ($vl_total_item*$percent_item)/100;
                        //Compara para caso haja diferença, atualizar para um valor mais "preciso"
                        if($value['iemipecentvalidado'] != number_format($percent_item, 2, '.','')){
                            $dados_item_d_evolucao[$key]['iemipecentvalidado'] = number_format($percent_item, 2, '.','');
                            $dados_item_d_evolucao[$key]['iemivlvalidado']     = number_format($vl_item_evo, 2, '.','');
                        }
                    }
                }
                $sum_valores_evo_d += $dados_item_d_evolucao[$key]['iemivlvalidado'];
            }

            //* 3 - Calcula o percentual da evolução de acordo com o valor executado e o valor total
            $percent_evo_d = ($sum_valores_evo_d*100)/$vl_total_d;
            $vl_evo_d      = ($vl_total_d*$percent_evo_d)/100;
            //* 4 - Compara os valores de dinheiro e percentual da evolução e ajusta caso necessário
            if($dados_evolucao['emipecentvalidadoedif'] != number_format($percent_evo_d, 2, '.','')){
               $dados_evolucao['emipecentvalidadoedif'] = number_format($percent_evo_d, 2, '.','');
               $dados_evolucao['emivlvalidadoedif']     = $vl_evo_d;
            }

            //08
            foreach ($dados_item_f_evolucao as $key => $value) {
                foreach ($arr_dados_f as $k => $v) {                        
                    if($v['icoid'] == $value['icoid']){                            
                        $vl_total_item  = $v['dadosCalculados']['vlTotalItem'];
                        $qtd_total_item = $v['dadosCalculados']['qtdTotalItem'];
                        $vl_item_evo    = $value['iemivlvalidado'];
                        //* 1 - Calcula o percentual executado do item de acordo com o valor da execução e o valor original do item
                        $percent_item = (!empty($vl_total_item) && $vl_total_item != '0.00' ) ? ($vl_item_evo*100)/$vl_total_item : 0;
                        $vl_item_evo  = ($vl_total_item*$percent_item)/100;
                        $qtd_item_evo = (!empty($vl_total_item) && $vl_total_item != '0.00' ) ? ($qtd_total_item*$vl_item_evo)/$vl_total_item : 0;

                        //Compara para caso haja diferença, atualizar para um valor mais "preciso"
                        if($value['iemipecentvalidado'] != number_format($percent_item, 2, '.','')){
                            $dados_item_f_evolucao[$key]['iemipecentvalidado'] = number_format($percent_item, 2, '.','');
                            $dados_item_f_evolucao[$key]['iemivlvalidado']     = number_format($vl_item_evo, 2, '.','');
                            $dados_item_f_evolucao[$key]['iemiqtdvalidado']    = number_format($qtd_item_evo, 2, '.','');
                        }
                    }
                }
                $sum_valores_evo_f += $dados_item_f_evolucao[$key]['iemivlvalidado'];
            }

            //* 3 - Calcula o percentual da evolução de acordo com o valor executado e o valor total
            $percent_evo_f = !empty($vl_total_f) ? ($sum_valores_evo_f*100)/$vl_total_f : 0;
            $vl_evo_f      = ($vl_total_f*$percent_evo_f)/100;

            //* 4 - Compara os valores de dinheiro e percentual da evolução e ajusta caso necessário
            if($dados_evolucao['emipecentvalidadoext'] != number_format($percent_evo_f, 2, '.','')){
               $dados_evolucao['emipecentvalidadoext'] = number_format($percent_evo_f, 2, '.','');
               $dados_evolucao['emivlvalidadoext']     = $vl_evo_f;
            }
        }
        else{
            //07
            foreach ($dados_item_d_evolucao as $key => $value) {
                foreach ($arr_dados_d as $k => $v) {                        
                    if($v['icoid'] == $value['icoid']){                            
                        $vl_total_item = $v['dadosCalculados']['vlTotalItem'];
                        $vl_item_evo   = $value['iemivlmedido'];
                        //* 1 - Calcula o percentual executado do item de acordo com o valor da execução e o valor original do item
                        $percent_item = (!empty($vl_total_item) && $vl_total_item != '0.00' ) ? ($vl_item_evo*100)/$vl_total_item : 0;
                        $vl_item_evo = ($vl_total_item*$percent_item)/100;
                        //Compara para caso haja diferença, atualizar para um valor mais "preciso"
                        if($value['iemipecentmedido'] != number_format($percent_item, 2, '.','')){
                            $dados_item_d_evolucao[$key]['iemipecentmedido'] = number_format($percent_item, 2, '.','');
                            $dados_item_d_evolucao[$key]['iemivlmedido']     = number_format($vl_item_evo, 2, '.','');
                        }
                    }
                }
                $sum_valores_evo_d += $dados_item_d_evolucao[$key]['iemivlmedido'];
            }

            //* 3 - Calcula o percentual da evolução de acordo com o valor executado e o valor total
            $percent_evo_d = empty($vl_total_d) ? 0 : ($sum_valores_evo_d*100)/$vl_total_d;
            $vl_evo_d      = ($vl_total_d*$percent_evo_d)/100;
            //* 4 - Compara os valores de dinheiro e percentual da evolução e ajusta caso necessário
            if($dados_evolucao['emipecentmedidoedif'] != number_format($percent_evo_d, 2, '.','')){
               $dados_evolucao['emipecentmedidoedif'] = number_format($percent_evo_d, 2, '.','');
               $dados_evolucao['emivlmedidoedif']     = $vl_evo_d;
            }

            //08
            foreach ($dados_item_f_evolucao as $key => $value) {
                foreach ($arr_dados_f as $k => $v) {                        
                    if($v['icoid'] == $value['icoid']){                            
                        $vl_total_item  = $v['dadosCalculados']['vlTotalItem'];
                        $qtd_total_item = $v['dadosCalculados']['qtdTotalItem'];
                        $vl_item_evo    = $value['iemivlmedido'];
                        //* 1 - Calcula o percentual executado do item de acordo com o valor da execução e o valor original do item
                        $percent_item = (!empty($vl_total_item) && $vl_total_item != '0.00' ) ? ($vl_item_evo*100)/$vl_total_item : 0;
                        $vl_item_evo  = ($vl_total_item*$percent_item)/100;
                        $qtd_item_evo = (!empty($vl_total_item) && $vl_total_item != '0.00' ) ? ($qtd_total_item*$vl_item_evo)/$vl_total_item : 0;

                        //Compara para caso haja diferença, atualizar para um valor mais "preciso"
                        if($value['iemipecentmedido'] != number_format($percent_item, 2, '.','')){
                            $dados_item_f_evolucao[$key]['iemipecentmedido'] = number_format($percent_item, 2, '.','');
                            $dados_item_f_evolucao[$key]['iemivlmedido']     = number_format($vl_item_evo, 2, '.','');
                            $dados_item_f_evolucao[$key]['iemiqtdmedido']    = number_format($qtd_item_evo, 2, '.','');
                        }
                    }
                }
                $sum_valores_evo_f += $dados_item_f_evolucao[$key]['iemivlmedido'];
            }

            //* 3 - Calcula o percentual da evolução de acordo com o valor executado e o valor total
            $percent_evo_f = (!empty($vl_total_f) && $vl_total_f != '0.00' ) ? ($sum_valores_evo_f*100)/$vl_total_f : 0;//($sum_valores_evo_f*100)/$vl_total_f;
            $vl_evo_f      = ($vl_total_f*$percent_evo_f)/100;

            //* 4 - Compara os valores de dinheiro e percentual da evolução e ajusta caso necessário
            if($dados_evolucao['emipecentmedidoext'] != number_format($percent_evo_f, 2, '.','')){
               $dados_evolucao['emipecentmedidoext'] = number_format($percent_evo_f, 2, '.','');
               $dados_evolucao['emivlmedidoext']     = $vl_evo_f;
            }
        }
        
        //09,10,11
        //Monta as querys e as executa
        switch ($tipo) {
            case 'salvar':
                $insert_evolucao_campos  = "";
                $insert_evolucao_valores = "";        
                foreach ($dados_evolucao as $key => $value) {
                    $insert_evolucao_campos  .= $key.",";
                    $insert_evolucao_valores .= $value.",";
                }
                $insert_evolucao = " INSERT INTO obras2.evolucaomi (".substr($insert_evolucao_campos,0,-1).") VALUES (".substr($insert_evolucao_valores,0,-1).") RETURNING emiid; ";

                try{
                    $emiid = $this->pegaUm($insert_evolucao);
                    $this->emiid = $emiid;
                } catch (Exception $ex) {
                    $this->rollback();
                    $arr_return['erro'] = true;
                    $arr_return['msg'] .= '\nErro ao cadastrar a Evolução MI. '.$ex->getMessage();
                }
                
                $docid = $this->atualizarDocidEvolucaoMi($emiid);
                
                foreach ($dados_item_d_evolucao as $key => $item) {
                    $dados_item_d_evolucao[$key]['emiid'] = $emiid;
                }
                
                foreach ($dados_item_d_evolucao as $key => $item) {
                    $insert_item_d_campos  = "";
                    $insert_item_d_valores = "";        
                    $insert_item_d = "";
                    foreach ($item as $k => $v) {
                        $insert_item_d_campos  .= $k.",";
                        $insert_item_d_valores .= $v.",";
                    }            
                    $insert_item_d = " INSERT INTO obras2.itemevolucaomi(".substr($insert_item_d_campos,0,-1).") VALUES (".substr($insert_item_d_valores,0,-1).") RETURNING iemiid; ";
                    try{
                        $iemiid = $this->pegaUm($insert_item_d);
                    } catch (Exception $ex) {
                        $this->rollback();
                        $arr_return['erro'] = true;
                        $arr_return['msg'] .= '\nErro ao cadastrar a os itens do cronograma da Evolução MI. '.$ex->getMessage();
                    }
                }
                
                foreach ($dados_item_f_evolucao as $key => $item) {
                    $dados_item_f_evolucao[$key]['emiid'] = $emiid;
                }
                
                foreach ($dados_item_f_evolucao as $key => $item) {
                    $insert_item_f_campos  = "";
                    $insert_item_f_valores = "";        
                    $insert_item_f = "";
                    foreach ($item as $k => $v) {
                        $insert_item_f_campos  .= $k.",";
                        $insert_item_f_valores .= $v.",";
                    }
                    $insert_item_f = " INSERT INTO obras2.itemevolucaomi(".substr($insert_item_f_campos,0,-1).") VALUES (".substr($insert_item_f_valores,0,-1).") RETURNING iemiid; ";

                    try{
                        $iemiid = $this->pegaUm($insert_item_f);
                    } catch (Exception $ex) {
                        $this->rollback();
                        $arr_return['erro'] = true;
                        $arr_return['msg'] .= '\nErro ao cadastrar a os itens de serviços externos da Evolução MI. '.$ex->getMessage();
                    }
                }
                
                if($arr_return['erro'] != true){
                   $arr_return['msg'] = 'Evolução MI cadastrada com sucesso!'; 
                }
                
                $this->commit();
                break;
            default:
                
                $update_evolucao_campos  = "";
                foreach ($dados_evolucao as $key => $value) {
                    $update_evolucao_campos  .= $key." = ".$value.',';            
                }
                $update_evolucao = " UPDATE obras2.evolucaomi SET ".substr($update_evolucao_campos,0,-1)." WHERE emiid = ".$emiid;

                try{
                    $this->executar($update_evolucao);
                } catch (Exception $ex) {
                    $this->rollback();
                    $arr_return['erro'] = true;
                    $arr_return['msg'] .= '\nErro ao '.$tipo.' a Evolução MI. Erro: '.$ex->getMessage();
                }
                
                $update_item_evolucao_campos  = "";
                foreach ($dados_item_d_evolucao as $key => $item) {
                    $update_item_evolucao_campos  = '';
                    foreach ($item as $k => $v) {
                        if($k === 'iemiid'){
                            $iemiid = $v;
                        }else{
                            $update_item_evolucao_campos  .= $k." = ".$v.',';
                        }
                    }
                    $update_item_evolucao = " UPDATE obras2.itemevolucaomi SET ".substr($update_item_evolucao_campos,0,-1)." WHERE iemiid = ".$iemiid;

                    try{
                        $this->executar($update_item_evolucao);
                    } catch (Exception $ex) {
                        $this->rollback();
                        $arr_return['erro'] = true;
                        $arr_return['msg'] .= '\nErro ao atualizar os itens do cronograma da Evolução MI. Erro: '.$ex->getMessage();
                    }
                }
                
                $update_item_evolucao_campos  = "";
                foreach ($dados_item_f_evolucao as $key => $item) {            
                    $update_item_evolucao_campos  = "";
                    foreach ($item as $k => $v) {
                        if($k === 'iemiid'){
                            $iemiid = $v;
                        }else{
                            $update_item_evolucao_campos  .= $k." = ".$v.',';
                        }
                    }
                    
                    $update_item_evolucao = " UPDATE obras2.itemevolucaomi SET ".substr($update_item_evolucao_campos,0,-1)." WHERE iemiid = ".$iemiid;

                    try{
                        $this->executar($update_item_evolucao);
                    } catch (Exception $ex) {
                        $this->rollback();
                        $arr_return['erro'] = true;
                        $arr_return['msg'] .= '\nErro ao atualizar a os itens de serviços externos da Evolução MI. Erro: '.$ex->getMessage();
                    }
                }
                
                if($arr_return['erro'] != true){
                   if($tipo == 'atualizar'){
                       $arr_return['msg'] = 'Evolução MI editada com sucesso!'; 
                   }else{
                       $arr_return['msg'] = 'Evolução MI validada com sucesso!';
                   }
                }
                
                $docid = $this->getDocidEvolucaoMi($emiid);                
                $this->commit();
                break;
        }
        
        $arr_return['emiid'] = $emiid;
        $arr_return['docid'] = $docid;
        
        return $arr_return;
    }
    
    public function excluirEvolucaoMi($emiid) {
        $this->validaParametroEmiid($emiid);
        if(empty($emiid)){ return 'Código da Evolução MI não definido'; }
        $sql = "UPDATE obras2.evolucaomi SET emistatus = 'I' WHERE emiid = ". $emiid;
        try{
            $this->executar($sql);
            $this->commit();
            return true;
        } catch (Exception $ex) {
            $this->rollback();
            return 'Erro ao excluir a Evolução. ERRO: '.$ex->getMessage();
        }
    }
    
    public function criarDocidEvolucaoMi($emiid) {
        $this->validaParametroEmiid($emiid);
        if(empty($emiid)){ return false; }
        require_once APPRAIZ . 'includes/workflow.php';
        $docdsc = "Fluxo da Evolução das Obras MI do módulo Obras II - emiid: " . $emiid;
        $docid = wf_cadastrarDocumento(TPDID_SUPERVISAO_MI, $docdsc);
        return $docid;
    }
    
    public function atualizarDocidEvolucaoMi($emiid){
        $this->validaParametroEmiid($emiid);
        if(empty($emiid)){ return false; }        
        $docid = $this->criarDocidEvolucaoMi($emiid);        
        if(empty($docid)){
            $this->rollback();
            return false;
        }else{
            $sql = 'UPDATE obras2.evolucaomi SET docid = '.$docid.' WHERE emiid = '.$emiid;
            try{
                $this->executar($sql);
            } catch (Exception $ex) {
                $this->rollback();
                return false;
            }
            $this->commit();
        }
        return $docid;
    }
    
    public function getDocidEvolucaoMi($emiid) {
        $this->validaParametroEmiid($emiid);
        if(empty($emiid)){ return false; }        
        $sql   = 'SELECT docid FROM obras2.evolucaomi  WHERE emiid = '.$emiid;
        $docid = $this->pegaUm($sql);
        return $docid;
    }
    
    /**
     * Busca na tabela obras2.itemevolucaomi os Itens do Cronograma - Utilizado quando já existe EVO MI
     * @param int $emiid
     * @param char(1) $relativoEdificacao
     * @return array
     */
    public function getItensEvolucaoMiByEmiidObrid($emiid, $obrid, $relativoEdificacao = 'D', $campos = null){
        $this->validaParametroObrid($obrid);
        if(empty($obrid)){ return array(); }
        $campos_v = $campos;
        $orderBy = '';
        if($campos == null){
            $campos = ' emi.*,
                        iemi.*,                    
                        ic.*,
                        um.umdeesc,
                        CASE 
                            WHEN ocr.ocrvalorexecucao > 0 THEN
                                    ((ico.icovlritem * 100) / ocr.ocrvalorexecucao)
                            ELSE
                                    0
                        END as icopercsobreobra,
                        ico.*,
                        ocr.ocrvalorexecucao';
            $orderBy = 'ORDER BY
                            ico.icoordem';
        }
        
        $sql = "SELECT
                        {$campos}
                FROM
                  obras2.evolucaomi emi
                JOIN obras2.itemevolucaomi iemi ON iemi.emiid = emi.emiid
                JOIN obras2.itenscomposicaoobra ico ON iemi.icoid = ico.icoid
                JOIN obras2.itenscomposicao ic ON ico.itcid = ic.itcid
                JOIN obras2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
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
               
                LEFT JOIN obras2.unidademedida um ON um.umdid = ico.umdid
                

                WHERE
                    ic.itcstatus = 'A' AND
                    cro.obrid = {$obrid} AND
                    ico.obrid = cro.obrid AND
                    ico.relativoedificacao = '{$relativoEdificacao}' AND
                    emi.emiid = {$emiid} AND
                    emi.emistatus = 'A'
                {$orderBy}";

        $dados = $this->carregar($sql);
        
        $dados = (!empty($dados)) ? $dados : array();
        
        if($campos_v == null){
            $arItens = $this->getDadosCalculosEvolucao($dados, $fonte = 'evo', $relativoEdificacao);
        }else{
            $arItens = $dados;
        }
        
        
        return $arItens;
    }
    
    /**
     * Busca na tabela obras2.itenscomposicaoobra os Itens do Cronograma - Utilizado quando ainda não existe EVO MI
     * @param int $obrid
     * @param char(1) $relativoEdificacao
     * @return array
     */
    public function getItemComposicaoObraByObra($obrid, $relativoEdificacao = 'D', $campos = null){
        
        $this->validaParametroObrid($obrid);
        if(empty($obrid)){ return array(); }
        
        $campos_v = $campos;
        $orderBy = '';
        if($campos == null){
            $campos = ' ic.*,
                        um.umdeesc,
                        CASE 
                            WHEN ocr.ocrvalorexecucao > 0 THEN
                                    ((ico.icovlritem * 100) / ocr.ocrvalorexecucao)
                            ELSE
                                    0
                        END as icopercsobreobra,
                        ico.*,
                        ocr.ocrvalorexecucao';
            $orderBy = 'ORDER BY
                            ico.icoordem';
        }
        
        $sql = "SELECT
                    {$campos}
                FROM
                    obras2.itenscomposicao ic
                JOIN obras2.itenscomposicaoobra ico ON ico.itcid = ic.itcid AND ico.icostatus = 'A'
                JOIN obras2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
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
               
                LEFT JOIN obras2.unidademedida um ON um.umdid = ico.umdid

                WHERE
                    ic.itcstatus = 'A' AND
                    cro.obrid = {$obrid} AND
                    ico.obrid = cro.obrid AND
                    ico.relativoedificacao = '{$relativoEdificacao}'
                {$orderBy}";
                    
        $dados = $this->carregar($sql);
        $dados = (!empty($dados)) ? $dados : array();
        
        if($campos_v == null){
            $arItens = $this->getDadosCalculosEvolucao($dados, 'ico', $relativoEdificacao);
        }else{
            $arItens = $dados;
        }
        
        return $arItens;
    }
    
    
    /**
     * Verificação de diferenças entre a quantidade de itens da última evolução, com os itens do cronograma.
     * @param type $id_ultima_evolucao
     * @return array
     */
    public function verificaDiferencasUltimaEvolucaoMiVsCronograma($id_ultima_evolucao, $obrid){
        $this->validaParametroEmiid($id_ultima_evolucao);
        $this->validaParametroObrid($obrid);
        $resp = array();
        $dados_cronograma_itens  = $this->getItemComposicaoObraByObra($obrid, 'D', ' ico.* ');
        $dados_cronograma_servex = $this->getItemComposicaoObraByObra($obrid, 'F', ' ico.* ');
        $dados_ultima_evo_itens  = $this->getItensEvolucaoMiByEmiidObrid($id_ultima_evolucao, $obrid, 'D', ' iemi.* ');
        $dados_ultima_evo_servex = $this->getItensEvolucaoMiByEmiidObrid($id_ultima_evolucao, $obrid, 'F', ' iemi.* ');
        //Diferença de QTD de Itens
        $difQtdIt = false;
        if(count($dados_cronograma_itens) != count($dados_ultima_evo_itens))  { $difQtdIt = true; $resp[] = array('tipo'=>'itens'); }        
        //Diferença de QTD de Itens de Serviços Externos
        $difQtdEx = false;
        if(count($dados_cronograma_servex) != count($dados_ultima_evo_servex)){ $difQtdEx = true; $resp[] = array('tipo'=>'servex'); }
        //Sem diferenças
        if($difQtdIt == false && $difQtdEx == false){ return false; }
        //Com diferenças
        return $resp;
    }
    
    /**
     * 
     * @param type $obrid
     * @return boolean (false)/integer (ID)
     */
    public function verificaUltimaEvolucaoMi($obrid) {
        $this->validaParametroObrid($obrid);
        if(empty($obrid)){ return false; }
        $response = $this->getUltEmiidByObrid($obrid);
        return $response;
    }
    
    public function verificaValidacaoEvolucaoMi($emiid) {
        $this->validaParametroEmiid($emiid);
        if(empty($emiid)){ return false; }
        $sql = "SELECT emivalidador
                FROM obras2.evolucaomi
                WHERE emiid = ".$emiid;
        $response = $this->pegaUm($sql);
        return $response;
    }
    
    /**
     * 
     * @param array $dados
     * @param type $fonte
     * @return type
     */
    public function getDadosCalculosEvolucao(array $dados_brutos, $fonte = 'ico'/*evo*/, $relativoEdificacao = 'D'){
      
        if(empty($dados_brutos)){return array();}
        
        $acao  = $_SESSION['obras2']['evoMiStatus'];
        $obrid = $_SESSION['obras2']['obrid'];
        $dados_manupulacao = array();
        
        /**
         * Foram feitos todos os cálculos de acordo com o passo-a-passo abaixo:
         * 00 - Separar os cálculos de acordo com a ação da tela e de acordo com o histórico
         *      - Cadastro  (pode, ou não, possuir uma evolução anterior) - se fonte = ico, não possui
         *      - Edição    (pode, ou não, possuir uma evolução anterior)
         *      - Validação (pode, ou não, possuir uma evolução anterior)
         *      - Impressao
         * 00.1 - Verificar se já existe evolução para esse OBRID -> $fonte dos dados != 'ico'
         * 01 - Recebe os dados de todas as etapas para manipulação;
         * 02 - Separar por etapas - foreach;
         * 03 - Fazer o cálculo de icovlritem do tipo 'F';
         * 04 - Fazer o cálculo do total somando os valores dos itens -> ocrvalorexecucao;
         * 05 - Fazer os ajustes de porcentagem dos itens;
         * 06 - Verificar a existência de uma evolução anterior;
         * 07 - Senão houver uma evolução anterior, o saldo é o máximo (100%) e o Acumulado é o mínimo (0%);
         * 08 - Se houver uma evolução anterior, recuperar os itens das evomi cadastradas anteriormente;
         * 09 - Verifica se possui dados de validação;
         * 10 - Se não possuir os dados, usar os dados dos campos de cadasto normal;
         * 11 - Se possuir os dados, usar os dados dos campos de validação;
         * 12 - Fazer a soma dos valores dos itens para obter os valores acumulados (iemipecentvalidado, iemivlvalidado, iemiqtdvalidado);
         * 13 - Fazer a cálculo dos saldos ;
         * 14 - Fazer os cálculos de porcentagem sobre a obra;
         */
        
        //01
        $dados_manupulacao = $dados_brutos;
        //00
        switch ($acao) {
            case 'cadastro':
                //00.1
                if($fonte == 'ico'){
                    $total_f = 0;
                    $total_d = 0;
                    foreach($dados_manupulacao as $key => $etapa){
                        //03
                        if($relativoEdificacao == 'F'){
                            $dados_manupulacao[$key]['icovlritem_total'] = $etapa['icovlritem']*$etapa['itcquantidade'];
                            $total_f += $etapa['icovlritem']*$etapa['itcquantidade'];
                        }
                        else{
                            $total_d += $etapa['icovlritem'];                
                        }
                    }
                    //04
                    foreach($dados_manupulacao as $key => $etapa){
                        if($relativoEdificacao == 'F'){
                            $dados_manupulacao[$key]['ocrvalorexecucao'] = $total_f;
                        }
                        else{
                            $dados_manupulacao[$key]['ocrvalorexecucao'] = $total_d;
                        }
                    }
                    //05
                    foreach ($dados_manupulacao as $key => $value) {
                        //Não faz os cálculos de validação, pois ainda não existe nada validado
                        if($value['relativoedificacao'] == 'D'){
                            $dados_manupulacao[$key]['icopercsobreobra']  = ($dados_manupulacao[$key]['ocrvalorexecucao'] != 0) ? ($value['icovlritem']*100)/$dados_manupulacao[$key]['ocrvalorexecucao'] : 0;
                            $dados_manupulacao[$key]['itempercsobreobra'] = ($value['icovlritem'] != 0) ? ($value['iemivlmedido']*$dados_manupulacao[$key]['icopercsobreobra'])/$value['icovlritem'] : 0;
                        }else{
                            $dados_manupulacao[$key]['icopercsobreobra']  = ($dados_manupulacao[$key]['ocrvalorexecucao'] != 0) ? ($value['icovlritem_total']*100)/$dados_manupulacao[$key]['ocrvalorexecucao'] : 0;
                            $dados_manupulacao[$key]['itempercsobreobra'] = ($value['icovlritem_total'] != 0) ? ($value['iemivlmedido']*$dados_manupulacao[$key]['icopercsobreobra'])/$value['icovlritem_total'] : 0;
                        }
                    }
                    //06
                    //Não faz os cálculos de saldo e valores acumulados, pois ainda não existe nada validado
                    //07 - Senão houver uma evolução anterior, o saldo é o máximo (100%) e o Acumulado é o mínimo (0%);
                    foreach($dados_manupulacao as $chave => $etapa){
                        $tipo_etapa          = $etapa['relativoedificacao'];
                        $icoid               = $etapa['icoid'];
                        $vl_total_item       = ($tipo_etapa == 'F') ? $etapa['icovlritem_total'] : $etapa['icovlritem'];
                        $qtd_total_item      = $etapa['itcquantidade'];
                        $prc_total_item_obra = $etapa['icopercsobreobra'];
        
                        $arr_retorno = array(
                            'icoid'         => $icoid,
                            'vlTotalItem'   => $vl_total_item,
                            'qtdTotalItem'  => $qtd_total_item,
                            'percTotalItem' => $prc_total_item_obra,
                            'sumVlValid'    => 0, // Valor Acumulado
                            'sumQtdValid'   => 0, // QTD Acumulada
                            'sumPercValid'  => 0, // Percent Acumulado
                            'saldoVl'       => $vl_total_item, // Saldo Valor a Executar
                            'saldoQtd'      => $qtd_total_item, // Saldo QTD a Executar
                            'saldoPerc'     => 100, // Saldo Percent a Executar
                            'sobreObraPerc' => 0 // Porcentagem do que foi executado sobre a obra (Percent Acumulado)
                        );
                        
                        $dados_manupulacao[$chave]['dadosCalculados'] = $arr_retorno;
                        unset($arr_retorno);
                    }
                    
                }
                else{
                    
                    $total_f = 0;
                    $total_d = 0;
                    foreach($dados_manupulacao as $key => $etapa){
                        //03
                        if($relativoEdificacao == 'F'){
                            $dados_manupulacao[$key]['icovlritem_total'] = $etapa['icovlritem']*$etapa['itcquantidade'];
                            $total_f += $etapa['icovlritem']*$etapa['itcquantidade'];
                        }
                        else{
                            $total_d += $etapa['icovlritem'];                
                        }
                    }
                    
                    //04
                    foreach($dados_manupulacao as $key => $etapa){
                        if($relativoEdificacao == 'F'){
                            $dados_manupulacao[$key]['ocrvalorexecucao'] = $total_f;
                        }
                        else{
                            $dados_manupulacao[$key]['ocrvalorexecucao'] = $total_d;
                        }
                    }
                    //05
                    foreach ($dados_manupulacao as $key => $value) {
                        if($value['relativoedificacao'] == 'D'){
                            $dados_manupulacao[$key]['icopercsobreobra']  = ($dados_manupulacao[$key]['ocrvalorexecucao'] != 0) ? ($value['icovlritem']*100)/$dados_manupulacao[$key]['ocrvalorexecucao'] : 0;
                            $dados_manupulacao[$key]['itempercsobreobra'] = ($value['icovlritem'] != 0) ? ($value['iemivlmedido']*$dados_manupulacao[$key]['icopercsobreobra'])/$value['icovlritem'] : 0;
                        }else{
                            $dados_manupulacao[$key]['icopercsobreobra']  = ($dados_manupulacao[$key]['ocrvalorexecucao'] != 0) ? ($value['icovlritem_total']*100)/$dados_manupulacao[$key]['ocrvalorexecucao'] : 0;
                            $dados_manupulacao[$key]['itempercsobreobra'] = ($value['icovlritem_total'] != 0) ? ($value['iemivlmedido']*$dados_manupulacao[$key]['icopercsobreobra'])/$value['icovlritem_total'] : 0;
                        }
                    }
                    //06 - Verificar a existência de uma evolução anterior;
                    foreach($dados_manupulacao as $chave => $etapa){
                        //ID da evolução anterior(cad->evo != cad->ico)
                        $emiid      = $etapa['emiid'];
                        $obrid      = $etapa['obrid'];
                        $tipo_etapa = $etapa['relativoedificacao'];
                        //Obra
                        $vl_total_obra       = $etapa['ocrvalorexecucao'];
                        //Item
                        $icoid               = $etapa['icoid'];
                        $vl_total_item       = ($tipo_etapa == 'F') ? $etapa['icovlritem_total'] : $etapa['icovlritem'];
                        $qtd_total_item      = $etapa['itcquantidade'];
                        $prc_total_item_obra = $etapa['icopercsobreobra'];

                        $arr_retorno = array(
                            'icoid'         => $icoid,
                            'vlTotalItem'   => $vl_total_item,
                            'qtdTotalItem'  => $qtd_total_item,
                            'percTotalItem' => $prc_total_item_obra,
                            'sumVlValid'    => 0, // Valor Acumulado
                            'sumQtdValid'   => 0, // QTD Acumulada
                            'sumPercValid'  => 0, // Percent Acumulado
                            'saldoVl'       => 0, // Saldo Valor a Executar
                            'saldoQtd'      => 0, // Saldo QTD a Executar
                            'saldoPerc'     => 0, // Saldo Percent a Executar
                            'sobreObraPerc' => 0, // Porcentagem do que foi executado sobre a obra (Percent Acumulado)
                        );
                        
                        // 08 - Há uma evolução anterior, recuperar os itens das evomi cadastradas anteriormente; ATENÇÂO: WHERE emiid <= '.$emiid.' 
                        $sql1 = 'SELECT emiid, emivalidador
                                 FROM obras2.evolucaomi
                                 WHERE emiid <= '.$emiid.' 
                                 AND obrid = '.$obrid.' 
                                 AND emistatus = \'A\' ';
                        $arrDados1 = $this->carregar($sql1);

                        $arrDados1 = (empty($arrDados1) || !is_array($arrDados1)) ? array() : $arrDados1 ;
                        
                        //09, 10, 11, 12
                        foreach ($arrDados1 as $key => $value) {
                            $sql2 = "SELECT iemi.iemipecentvalidado, 
                                            iemi.iemivlvalidado,
                                            iemi.iemiqtdvalidado,
                                            
                                            iemi.iemipecentmedido, 
                                            iemi.iemivlmedido,
                                            iemi.iemiqtdmedido
                                            
                                     FROM obras2.itemevolucaomi   iemi
                                     INNER JOIN obras2.evolucaomi emi  ON iemi.emiid = emi.emiid
                                     WHERE iemi.icoid      = ".$icoid."
                                     AND   iemi.emiid      = ".$value["emiid"]." 
                                     AND   emi.emistatus   = 'A'
                                     AND   iemi.iemistatus = 'A' 
                                     ORDER BY iemi.iemiid DESC";
                            $arrDados2 = $this->pegaLinha($sql2);
                            
                            if(!empty($arrDados2)){
                                // 09 - Verifica se possui dados de validação;
//                                if(!empty($arrDados2['iemipecentvalidado']) && $arrDados2['iemipecentvalidado'] != '0.00' && $arrDados2['iemipecentvalidado'] != '0,00' ){
                                if(!empty($value['emivalidador'])){
                                    //11 - Se possuir os dados, usar os dados dos campos de validação;
                                    //12 - Fazer a soma dos valores dos itens para obter os valores acumulados
                                    $arr_retorno['sumPercValid'] += $arrDados2['iemipecentvalidado'];
                                    $arr_retorno['sumVlValid']   += $arrDados2['iemivlvalidado'];
                                    $arr_retorno['sumQtdValid']  += $arrDados2['iemiqtdvalidado'];
                                }else{
                                    //10 - Se não possuir os dados, usar os dados dos campos de cadasto normal;
                                    //12 - Fazer a soma dos valores dos itens para obter os valores acumulados
                                    $arr_retorno['sumPercValid'] += $arrDados2['iemipecentmedido'];
                                    $arr_retorno['sumVlValid']   += $arrDados2['iemivlmedido'];
                                    $arr_retorno['sumQtdValid']  += $arrDados2['iemiqtdmedido'];
                                }
                            }
                        }
                        
                        //13 - Fazer a cálculo dos saldos ;
                        $arr_retorno['saldoVl']   = $vl_total_item  - $arr_retorno['sumVlValid'];
                        $arr_retorno['saldoQtd']  = $qtd_total_item - $arr_retorno['sumQtdValid'];
                        $arr_retorno['saldoPerc'] = 100             - $arr_retorno['sumPercValid'];
                        
                        if($arr_retorno['saldoVl'] < 0)  { $arr_retorno['saldoVl']   = 0; }
                        if($arr_retorno['saldoQtd'] < 0) { $arr_retorno['saldoQtd']  = 0; }
                        if($arr_retorno['saldoPerc'] < 0){ $arr_retorno['saldoPerc'] = 0; }

                        //14 - Fazer os cálculos de porcentagem sobre a obra;
                        if($vl_total_obra != 0){
                            $arr_retorno['sobreObraPerc'] = ($arr_retorno['sumVlValid']*100)/$vl_total_obra;
                        }else{
                            $arr_retorno['sobreObraPerc'] = 0;
                        }

                        $dados_manupulacao[$chave]['dadosCalculados'] = $arr_retorno;
                        
                        unset($arr_retorno);
                    }
                    
                }


                break;
/*                
//            case 'edicao':
//                ver DEFAULT
//                break;
//            case 'validacao':
//                ver DEFAULT
//                break;
//            case 'impressao':
//                ver DEFAULT
//                break;
*/
            default:
                $total_f = 0;
                    $total_d = 0;
                    foreach($dados_manupulacao as $key => $etapa){
                        //03
                        if($relativoEdificacao == 'F'){
                            $dados_manupulacao[$key]['icovlritem_total'] = $etapa['icovlritem']*$etapa['itcquantidade'];
                            $total_f += $etapa['icovlritem']*$etapa['itcquantidade'];
                        }
                        else{
                            $total_d += $etapa['icovlritem'];                
                        }
                    }
                    //04
                    foreach($dados_manupulacao as $key => $etapa){
                        if($relativoEdificacao == 'F'){
                            $dados_manupulacao[$key]['ocrvalorexecucao'] = $total_f;
                        }
                        else{
                            $dados_manupulacao[$key]['ocrvalorexecucao'] = $total_d;
                        }
                    }
                    //05
                    foreach ($dados_manupulacao as $key => $value) {
                        if($value['relativoedificacao'] == 'D'){
                            $dados_manupulacao[$key]['icopercsobreobra']  = ($dados_manupulacao[$key]['ocrvalorexecucao'] != 0) ? ($value['icovlritem']*100)/$dados_manupulacao[$key]['ocrvalorexecucao'] : 0;
                            $dados_manupulacao[$key]['itempercsobreobra'] = ($value['icovlritem'] != 0) ? ($value['iemivlmedido']*$dados_manupulacao[$key]['icopercsobreobra'])/$value['icovlritem'] : 0;
                            
                            if($acao == 'validacao'){
                                $dados_manupulacao[$key]['itempercsobreobra_validacao'] = ($value['icovlritem'] != 0) ? ($value['iemivlvalidado']*$dados_manupulacao[$key]['icopercsobreobra'])/$value['icovlritem'] : 0;
                            }
                            
                            
                        }else{
                            $dados_manupulacao[$key]['icopercsobreobra']  = ($dados_manupulacao[$key]['ocrvalorexecucao'] != 0) ? ($value['icovlritem_total']*100)/$dados_manupulacao[$key]['ocrvalorexecucao'] : 0;
                            $dados_manupulacao[$key]['itempercsobreobra'] = ($value['icovlritem_total'] != 0) ? ($value['iemivlmedido']*$dados_manupulacao[$key]['icopercsobreobra'])/$value['icovlritem_total'] : 0;
                            
                            if($acao == 'validacao'){
                                $dados_manupulacao[$key]['itempercsobreobra_validacao'] = ($value['icovlritem'] != 0) ? ($value['iemivlvalidado']*$dados_manupulacao[$key]['icopercsobreobra'])/$value['icovlritem'] : 0;
                            }
                        }
                    }
                    //06 - Verificar a existência de uma evolução anterior;
                    $passada = 0;
                    foreach($dados_manupulacao as $chave => $etapa){
//                        ver($etapa);
                        //ID da evolução atual(edição)
                        $emiid      = ($fonte == 'ico') ? $_GET['emiid'] : $etapa['emiid'];
                        $obrid      = $etapa['obrid'];
                        $tipo_etapa = $etapa['relativoedificacao'];
                        //Obra
                        $vl_total_obra       = $etapa['ocrvalorexecucao'];
                        //Item
                        $icoid               = $etapa['icoid'];
                        $vl_total_item       = ($tipo_etapa == 'F') ? $etapa['icovlritem_total'] : $etapa['icovlritem'];
                        $qtd_total_item      = $etapa['itcquantidade'];
                        $prc_total_item_obra = $etapa['icopercsobreobra'];

                        $arr_retorno = array(
                            'icoid'         => $icoid,
                            'vlTotalItem'   => $vl_total_item,
                            'qtdTotalItem'  => $qtd_total_item,
                            'percTotalItem' => $prc_total_item_obra,
                            'sumVlValid'    => 0, // Valor Acumulado
                            'sumQtdValid'   => 0, // QTD Acumulada
                            'sumPercValid'  => 0, // Percent Acumulado
                            'saldoVl'       => 0, // Saldo Valor a Executar
                            'saldoQtd'      => 0, // Saldo QTD a Executar
                            'saldoPerc'     => 0, // Saldo Percent a Executar
                            'sobreObraPerc' => 0, // Porcentagem do que foi executado sobre a obra (Percent Acumulado)
                        );
                        
                        // 08 - Verifica se há uma evolução anterior, recuperar os itens das evomi cadastradas anteriormente; 
                        // ATENÇÂO: WHERE emiid < '.$emiid.'
						$arrDados1 = array();
                        if(!empty($emiid)){
							$sql1 = 'SELECT emiid
                                 FROM obras2.evolucaomi
                                 WHERE emiid < '.$emiid.'
                                 AND obrid = '.$obrid.'
                                 AND emistatus = \'A\' ';

							$arrDados1 = $this->carregar($sql1);
						}


                        $arrDados1 = (empty($arrDados1) || !is_array($arrDados1)) ? array() : $arrDados1 ;
        
                        //09, 10, 11, 12
                        foreach ($arrDados1 as $key => $value) {
                            $sql2 = "SELECT iemi.iemipecentvalidado, 
                                            iemi.iemivlvalidado,
                                            iemi.iemiqtdvalidado,
                                            
                                            iemi.iemipecentmedido, 
                                            iemi.iemivlmedido,
                                            iemi.iemiqtdmedido
                                            
                                     FROM obras2.itemevolucaomi   iemi
                                     INNER JOIN obras2.evolucaomi emi  ON iemi.emiid = emi.emiid
                                     WHERE iemi.icoid      = ".$icoid."
                                     AND   iemi.emiid      = ".$value["emiid"]." 
                                     AND   emi.emistatus   = 'A'
                                     AND   iemi.iemistatus = 'A' ";
                            $arrDados2 = $this->pegaLinha($sql2);

                            if(!empty($arrDados2)){
                                // 09 - Verifica se possui dados de validação;
                                if(!empty($arrDados2['iemipecentvalidado'])){
                                    //11 - Se possuir os dados, usar os dados dos campos de validação;
                                    //12 - Fazer a soma dos valores dos itens para obter os valores acumulados
                                    $arr_retorno['sumPercValid'] += $arrDados2['iemipecentvalidado'];
                                    $arr_retorno['sumVlValid']   += $arrDados2['iemivlvalidado'];
                                    $arr_retorno['sumQtdValid']  += $arrDados2['iemiqtdvalidado'];
                                }else{
                                    //10 - Se não possuir os dados, usar os dados dos campos de cadasto normal;
                                    //12 - Fazer a soma dos valores dos itens para obter os valores acumulados
                                    $arr_retorno['sumPercValid'] += $arrDados2['iemipecentmedido'];
                                    $arr_retorno['sumVlValid']   += $arrDados2['iemivlmedido'];
                                    $arr_retorno['sumQtdValid']  += $arrDados2['iemiqtdmedido'];
                                }
                            }
                        }
                        
                        //13 - Fazer a cálculo dos saldos ;
                        $arr_retorno['saldoVl']   = $vl_total_item  - $arr_retorno['sumVlValid'];
                        $arr_retorno['saldoQtd']  = $qtd_total_item - $arr_retorno['sumQtdValid'];
                        $arr_retorno['saldoPerc'] = 100             - $arr_retorno['sumPercValid'];
                        
                        //14 - Fazer os cálculos de porcentagem sobre a obra;
                        if($vl_total_obra != 0){
                            $arr_retorno['sobreObraPerc'] = ($arr_retorno['sumVlValid']*100)/$vl_total_obra;
                        }else{
                            $arr_retorno['sobreObraPerc'] = 0;
                        }
                        
                        $dados_manupulacao[$chave]['dadosCalculados'] = $arr_retorno;
                        
                        unset($arr_retorno);
                    }
                break;
        }
        
        return $dados_manupulacao;
    }
    
    public function getEstadoWfEvolucaoMI($emiid) {
        $campos         = 'ed.esdid, ed.esddsc';
        $param['emiid'] = $emiid;
        $sql            = $this->listaEvolucaoCompletaSql($param, $campos);
        $estado         = $this->pegaLinha($sql); 
        return $estado;
    }
    
    /**
     * Recebe o docid da Evolução MI e retorna a linha com os dados da última tramitação da Evolução
     * @param int $docid 
     * @return array $dados_tramitacao
     */
    public function getDadosTramitacaoEvolucaoMi($docid = null, $campos = null){
        
        if($docid == null){
           $docid = $this->docid; 
           if($docid == null){
               return array();
           }
        }
        
        if(empty($campos)){
            $campos = " emi.obrid, emi.emiid, 
                        emi.usucpf as usu_cpf_cad_emi,
                        emi.docid, 
                        usu_os.usunome as usu_nome_cad_emi,
                        tpd.tpddsc, doc.docdsc,
                        esd.esdid, esd.esddsc, hst.hstid, 
                        hst.usucpf       as usu_cpf_exec_tramitacao, 
                        usu_acao.usunome as usu_nome_exec_tramitacao, 
                        usu_acao.usuemail as usu_email_exec_tramitacao,
                        hst.htddata, aed.aedid,
                        aed.aeddscrealizada, 
                        aed.esdidorigem, aed.esdiddestino,
                        esdorigem.esddsc as estado_de_origem, esddestino.esddsc as estado_de_destino,
                        COALESCE(to_char(hst.htddata, 'DD/MM/YYYY')) AS data_tramitacao,
                        COALESCE(to_char(hst.htddata, 'YYYY-MM-DD')) AS data_tramitacao2
                        ";
        }
        
        $sql = " SELECT 
                        {$campos}
                            
                 FROM obras2.evolucaomi                 emi
                 
                 LEFT JOIN workflow.documento          doc ON doc.docid  = emi.docid
                 LEFT JOIN workflow.tipodocumento      tpd ON tpd.tpdid  = doc.tpdid
                 LEFT JOIN workflow.estadodocumento    esd ON esd.esdid  = doc.esdid
                 LEFT JOIN workflow.historicodocumento hst ON hst.hstid  = doc.hstid
                 LEFT JOIN workflow.acaoestadodoc      aed ON aed.aedid  = hst.aedid                 
                 LEFT JOIN seguranca.usuario        usu_os ON emi.usucpf = usu_os.usucpf 
                 LEFT JOIN seguranca.usuario      usu_acao ON hst.usucpf = usu_acao.usucpf 
                 
                 LEFT JOIN workflow.estadodocumento esdorigem  ON aed.esdidorigem  = esdorigem.esdid
                 LEFT JOIN workflow.estadodocumento esddestino ON aed.esdiddestino = esddestino.esdid

                 WHERE doc.docid = ".$docid."
                   AND emi.emistatus = 'A'
                "; 
        
        $dados_tramitacao = $this->pegaLinha($sql);        
        $dados_tramitacao = ($dados_tramitacao) ? $dados_tramitacao  : array();
        
        return $dados_tramitacao;
    }
    
    public function posAcaoTramiteEvolucaoMI($emiid) {
        
        $this->carregarPorId( $emiid );
        $arr_dados_evoMi         = $this->getDados();        
        $docid                   = $arr_dados_evoMi['docid'];        
        $arr_dados_tramite_evoMi = $this->getDadosTramitacaoEvolucaoMi($docid);
        //Envia o e-mail
        $this->enviaEmailTramiteEvolucaoMi($emiid, $arr_dados_tramite_evoMi);
        
        //Caso necessário, mais ações podem ser colocadas aqui;
        
    }
    
    /**
     * Métodos de envio de e-mail quando o Estado do WF da Evolução mudar.
     */
    public function enviaEmailTramiteEvolucaoMi($emiid, $arr_dados_tramite_evoMi) {
        // Monta o filtro de Perfis Responsáveis por cada ação do Workflow
        switch ($arr_dados_tramite_evoMi['aedid']) {
            //Em cadastramento: Enviar para validação
            case 2448:
                $filtro_resp = ' '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE;
                break;
            //Validação: Validar
            case 2449:
                $filtro_resp = ' '.PFLCOD_EMPRESA_MI_GESTOR.', '.PFLCOD_EMPRESA_MI_FISCAL;
                break;
            //Validação: Enviar para Correção
            case 2450:
                $filtro_resp = ' '.PFLCOD_EMPRESA_MI_GESTOR.', '.PFLCOD_EMPRESA_MI_FISCAL;
                break;
            //Correção: Enviar para Validação
            case 2451:
                $filtro_resp = ' '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE;
                break;
            default:
                $filtro_resp = ' '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE;
                break;
        }
        
        $dados_responsaveis = $this->getDadosResponsaveisTramitacaoEvolucaoMi($emiid, $filtro_resp);
        
        // Faz o envio do email do tramite da Evolução MI
        if(!empty($dados_responsaveis)){
            foreach ($dados_responsaveis as $key => $usuario) {
                $destinatario = array($usuario['email']);
                $dados_email  = $this->montaCorpoEmailTamiteEvolucaoMi($emiid, $arr_dados_tramite_evoMi, $usuario);                
                $email        = new Email();
                $email->popularDadosObjeto($dados_email);                    
                $email->salvar($destinatario);
                $email->enviar();
            }            
        }else{
            $destinatario = array('');
            $dados_email  = $this->montaCorpoEmailTamiteEvolucaoMi($emiid, $arr_dados_tramite_evoMi, array());
            $email        = new Email();
            $email->popularDadosObjeto($dados_email);                    
            $email->salvar($destinatario);
            // Não enviar, apenas salvar até segunda ordem.            
        }
        return true;
    }
    /**
     * 
     * @param int $emiid
     * @param array $array_dados
     * @param array $responsavel_destino
     * @return array no formato de envio do Email.class.inc
     */
    public function montaCorpoEmailTamiteEvolucaoMi($emiid, $array_dados = array(), $responsavel_destino = array()) {
        $arr_sup   = $this->getDadosEvolucaoMi($emiid, array('o.obrid','o.obrnome'));
        $nome_obra = '('.$arr_sup['obrid'].')'.$arr_sup['obrnome'];
        $obrid     = $arr_sup['obrid'];
        
        $data_tramitacao    = $array_dados['data_tramitacao'];
        $nome_tramitador    = $array_dados['usu_nome_exec_tramitacao'];
        $email_tramitador   = $array_dados['usu_email_exec_tramitacao'];
        $estado_atual_os    = $array_dados['estado_de_destino'];
        $estado_anterior_os = $array_dados['estado_de_origem'];
        $acao_realizada     = $array_dados['aeddscrealizada'];
        $email = new Email;
        $dt   = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        
        if(empty($responsavel_destino)){
            $nome_responsavel   = 'Usuário não encontrado';
            $cpf_responsavel    = '000000';
            $email_responsavel  = '';
        }else{
            $nome_responsavel   = $responsavel_destino['nome'];
            $cpf_responsavel    = $responsavel_destino['cpf'];
            $email_responsavel  = $responsavel_destino['email'];            
        }
        
        $html = ' 
                <html>
                    <head>
                        <title></title>
                    </head>
                    <body>
                        <table style="width: 100%;">
                            <thead>
                                <tr>
                                    <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                        <p><img  src="data:image/png;base64,'. $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        '.$data.'
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        &nbsp;
                                    </td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px;" colspan="2">
                                        <b> TRAMITAÇÃO DE EVOLUÇÃO MI </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: SIMEC - EVOLUÇÃO MI - ID '.$emiid.' - Tramitada
                                    </td>
                                </tr>

                                <tr>
                                    <td>Nome do Responsável:</td>
                                    <td>'.$nome_responsavel.'</td>
                                </tr>
                                <tr>
                                    <td>CPF do Responsável:</td>
                                    <td>'.$cpf_responsavel.'</td>
                                </tr>
                                <tr>
                                    <td>Obra:</td>
                                    <td>'.$nome_obra.'</td>
                                </tr>                            
                                <tr>
                                    <td>Estado Atual:</td>
                                    <td><b> '.$estado_atual_os.' </b></td>
                                </tr>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                                <tr>
                                    <td>Estado Anterior:</td>
                                    <td><b> '.$estado_anterior_os.' </b></td>
                                </tr>
                                <tr>
                                    <td>Data da Tramitação:</td>
                                    <td>'.$data_tramitacao.'</td>
                                </tr>
                                <tr>
                                    <td>Quem tramitou:</td>
                                    <td> '.$nome_tramitador.' - ('.$email_tramitador.')</td>
                                </tr>
                                <tr>
                                    <td>Ação realizada:</td>
                                    <td> <b>'.$acao_realizada.'</b></td>
                                </tr>                        
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                        <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                    </td>
                                </tr>
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                            </tfoot>
                        </table>
                    </body>
                </html>
                ';
        
            $assunto            = "SIMEC - TRAMITAÇÃO DE EVOLUÇÃO MI - ID ".$emiid." - Tramitada";
            $conteudo           = $html;
            
            $dados = array(
                            'usucpf'               => $_SESSION['usucpf'], 
                            'emlconteudo'          => $conteudo,
                            'emlassunto'           => $assunto, 
                            'temid'                => 12,
                            'emlregistroatividade' => true,
                            'obrid'                => $obrid
                          );

            return $dados;
    }

    public function getDadosResponsaveisTramitacaoEvolucaoMi($emiid, $filtro_resp){
        
        $arr = $this->getDadosEvolucaoMi($emiid);
        
        if(!empty($arr)){
            $empid = $arr['empid'];            
        }else{
            return array();
        }
        
        if(trim($filtro_resp) == ''){
            return array();            
        }
        
        $filtro_sql_resp = " AND ur.pflcod IN (".$filtro_resp.")";

        $sql_responsaveis = "SELECT DISTINCT
                                         su.usucpf   as cpf,
                                         su.usunome  as nome,
                                         su.usuemail as email
                             FROM seguranca.usuario su
                             INNER JOIN obras2.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf AND ur.rpustatus = 'A'
                             WHERE
                                     ur.empid = {$empid}
                                     {$filtro_sql_resp}
                             ORDER BY
                                     su.usunome";

        $dados_responsaveis = $this->carregar($sql_responsaveis);
        $dados_responsaveis = (is_array($dados_responsaveis) && count($dados_responsaveis) > 0) ? $dados_responsaveis : array();
        
        return $dados_responsaveis;
    }
    
    public function getDadosUsuarioCpf($cpf = NULL) {
        if($cpf == NULL){
            $cpf = $_SESSION['usucpf'];
        }
        $cpf = (string)$cpf;
        $sql = "SELECT usunome, usucpf, usuemail
                FROM seguranca.usuario 
                WHERE usucpf = '".$cpf."'";
        $dados_usuario = $this->pegaLinha($sql);
        $dados_usuario = ($dados_usuario) ? $dados_usuario : array();
        return $dados_usuario;
    }
    
}