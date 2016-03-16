<?php

class MonitoramentoEspecial extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array( '' => null );
    
    /**
     * 
     * MÉTODOS DAS ATIVIDADES DE MONITORAMENTO
     * 
     */    
    public function montaListaAtividadesMonitoramento($params = array(), $tipo = 'html'/*xls*/, $coluna_acoes = true ) {
        
        $cabecalho = array( "Ações",'Obra','Entidade','ID da Atividade','Origem da Atividade','Observação','Tipo da Atividade','Subtipo da Atividade',
                            'UF','Município','Data de Cadastro','Data de Limite','Situação da Atividade', 'Responsável pelo Cadastro');
        
        if(possuiPerfil(array(PFLCOD_SUPER_USUARIO))){
            $acoes  = ' 
                        \'<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="javascript: alterarAtividade(\'|| atm.atmid ||\');" title="Alterar">
                          <img src="/imagens/send.png" title="Duplicar Atividade" onclick="javascript: duplicarAtividade(\'|| atm.atmid ||\')" style="cursor: pointer;" title="Duplicar Atividade">
                          <img style="cursor: pointer; vertical-align:middle" src="../imagens/gif_inclui.gif" onclick="javascript: incluirTarefaAtividade(\'|| atm.atmid ||\');" title="Adicionar Tarefa">
                          <img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer; margin-left: 3px;" onclick="javascript: excluirAtividade(\'|| atm.atmid ||\');" title="Excluir">\'
                         as acoes,
                      ';
        }else{
            $acoes  = ' \'<img align="absmiddle" src="/imagens/consultar.gif" style="cursor: pointer" onclick="javascript: visualizarAtividade(\'|| atm.atmid ||\');" title="Ver Atividade">\' as acoes, ';
        }
        
        if($tipo == 'xls'){
            $acoes  = " '' as acoes, ";
        }
        
        if($coluna_acoes == false){
            $acoes  = " '' as acoes, ";
        }
        
        $campos = " {$acoes}
                    '(' || obr.obrid || ')' || obr.obrnome as obra,
                    --'(' || atm.entid || ')' || ent.entnome as entidade,
                    ent.entnome as entidade,
                    atm.atmid,
                    atm.atmnome,
                    atm.atmobs,
                    tam.tamdesc,
                    sam.samdesc,
                    est.estuf,
                    mun.mundescricao,
                    COALESCE(TO_CHAR(atm.atmdtinclusao, 'dd/mm/YYYY')) AS atmdtinclusao,
                    COALESCE(TO_CHAR(atm.atmdtlimiteconclusao, 'dd/mm/YYYY')) AS atmdtlimiteconclusao,
                    esd.esddsc,
                    usu.usunome
                  ";
        
        $sql = $this->getDadosAtividade($params, 'sql', $campos);
        
        if ($tipo == 'xls') {            
            ob_clean();
            ini_set("memory_limit", "512M");
            header("Content-type: application/excel; name=ListaDeAtividadesDeMonitoramentoEspecial.xls");
            header("Content-Disposition: attachment; filename=ListaDeAtividadesDeMonitoramentoEspecial.xls");
            $this->sql_to_xml_excel($sql, 'ListaDeAtividadesDeMonitoramentoEspecial', $cabecalho);
        } else {
            $this->monta_lista($sql, $cabecalho, 30, 10, 'N', 'center', 'N', 'N');
        }
    }
    
    public function getDadosAtividade($param = array(), $tipo_retorno = 'sql'/*array*/, $campos = null) {
        
        $param = $this->getFiltrosJoinsDadosAtividadeMonitoramento($param);
        $where = $param['where'];
        $join  = $param['join'];
        
        array_unshift($where, " atm.atmstatus = 'A' ");
        
        $strCampos = empty($campos) ? ' * ' : $campos;
        $strWhere  = implode(' AND ', $where);
        $strJoin   = implode('  ', $join);
        
        
        $sql = "SELECT {$strCampos} 
                FROM obras2.atividademonitoramento atm
                {$strJoin}
                WHERE {$strWhere}
                ";
                
        if($tipo_retorno == 'sql'){
            return $sql;
        }else{
            $arrDados = $this->carregar($sql);
            $arrDados = !empty($arrDados) ? $arrDados : array() ;
            return $arrDados;
        }
        
    }
    
    public function getFiltrosJoinsDadosAtividadeMonitoramento($param) {
        if(empty($param)){
            $params = array('where' => array(), 'join'=>array());
            return $params;
        }
        
        $where = array();
        $join  = array();
        
        foreach ($param as $key => $value) {
            $value = is_string($value) ? trim($value) : $value;
            switch (true) {
                case ($key == 'atmid' && !empty($value)):
                    $where[] = " atm.atmid = ".$value;
                    break;
                case ($key == 'obrbuscatexto' && !empty($value)): 
                        $valueTemp = removeAcentos(str_replace("-", " ", (trim($value))));
                        $where[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $valueTemp . "%') OR obr.obrid::CHARACTER VARYING ILIKE ('%" . $value . "%') ) ";
                    break;
                case ($key == 'cnpj' && !empty($value)): 
                    // Verificar filtro na segunda
                    //$where[] = " atm. = ".$value; //  54.636.435/6353-43
                    //$join[]  = " . = ".$value; 
                    break;
                case ($key == 'atmnome' && !empty($value)): 
                    $where[] = " atm.atmnome = '".$value."'"; //  sdffdsgs
                    break;
                case ($key == 'tamid' && !empty($value)): 
                    $where[] = " atm.tamid = ".$value; //  1
                    break;
                case ($key == 'samid' && !empty($value)): 
                    $where[] = " atm.samid = ".$value; //  2
                    break;
                case ($key == 'esdid' && !empty($value)): 
                    $where[] = " esd.esdid = ".$value; //  1238
                    break;
                case ($key == 'usucpf_inclusao' && !empty($value)):
                    if (is_array($value)) {
                        foreach ($value as $k => $v) {
                            $value[$k] = "'{$v}'";
                        }
                    } else {
                        $value = array("'{$value}'");
                    }
                    foreach ($value as $k => $v) {
                        if (trim($v) == '' || $v == "''" || empty($v)) {
                            unset($value[$k]);
                        }
                    }
                    if (!empty($value)) {
                        $where[] = " atm.usucpf_inclusao IN ( " . implode(",", $value) . " ) ";
                    }
                    break;
                case ($key == 'estuf' && !empty($value)): 
                    $where[] = " atm.estuf = '".$value."'"; //  DF
                    break;
                case ($key == 'muncod' && !empty($value)): 
                    $where[] = " atm.muncod = '".$value."'"; //  5300108
                    break;
                case ($key == 'entid' && !empty($value)): 
                    $where[] = " atm.entid = ".$value; //  606965
                    break;
                default :
                    break;
            }
        }
        
        if(!empty($param['atmdtinclusao_de']) && !empty($param['atmdtinclusao_ate'])){
            $de  = explode('/', $param['atmdtinclusao_de']);
            $ate = explode('/', $param['atmdtinclusao_ate']);
            $param['atmdtinclusao_de']  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $param['atmdtinclusao_ate'] = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " atm.atmdtinclusao BETWEEN '{$param['atmdtinclusao_de']}' AND '{$param['atmdtinclusao_ate']}' ";
        }elseif(!empty($param['atmdtinclusao_de']) && empty($param['atmdtinclusao_ate'])){
            $de  = explode('/', $param['atmdtinclusao_de']);
            $param['atmdtinclusao_de']  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " atm.atmdtinclusao = '{$param['atmdtinclusao_de']}' ";
        }elseif(empty($param['atmdtinclusao_de']) && !empty($param['atmdtinclusao_ate'])){
            $ate = explode('/', $param['atmdtinclusao_ate']);
            $param['atmdtinclusao_ate'] = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " atm.atmdtinclusao = '{$param['atmdtinclusao_ate']}' ";
        }
        
        if(!empty($param['atmdtlimiteconclusao_de']) && !empty($param['atmdtlimiteconclusao_ate'])){
            $de  = explode('/', $param['atmdtlimiteconclusao_de']);
            $ate = explode('/', $param['atmdtlimiteconclusao_ate']);
            $param['atmdtlimiteconclusao_de']  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $param['atmdtlimiteconclusao_ate'] = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " atm.atmdtlimiteconclusao BETWEEN '{$param['atmdtlimiteconclusao_de']}' AND '{$param['atmdtlimiteconclusao_ate']}' ";
        }elseif(!empty($param['atmdtlimiteconclusao_de']) && empty($param['atmdtlimiteconclusao_ate'])){
            $de  = explode('/', $param['atmdtlimiteconclusao_de']);
            $param['atmdtlimiteconclusao_de']  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " atm.atmdtlimiteconclusao = '{$param['atmdtlimiteconclusao_de']}' ";
        }elseif(empty($param['atmdtlimiteconclusao_de']) && !empty($param['atmdtlimiteconclusao_ate'])){
            $ate = explode('/', $param['atmdtlimiteconclusao_ate']);
            $param['atmdtlimiteconclusao_ate'] = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " atm.atmdtlimiteconclusao = '{$param['atmdtlimiteconclusao_ate']}' ";
        }
        
        $join[] = " INNER JOIN obras2.tipoatividademonitoramento    tam ON atm.tamid  = tam.tamid ";
        $join[] = " INNER JOIN obras2.subtipoatividademonitoramento sam ON atm.samid  = sam.samid ";
        $join[] = " INNER JOIN territorios.estado                   est ON atm.estuf  = est.estuf ";
        $join[] = " INNER JOIN territorios.municipio                mun ON atm.muncod = mun.muncod ";
        $join[] = " INNER JOIN entidade.entidade                    ent ON atm.entid  = ent.entid ";
        $join[] = " LEFT JOIN obras2.obras                          obr ON atm.obrid  = obr.obrid ";
        $join[] = " LEFT JOIN workflow.documento 	            doc ON doc.docid  = atm.docid AND doc.tpdid = " .TPID_ME_ATIVIDADE_MONITORAMENTO." ";
        $join[] = " LEFT JOIN workflow.estadodocumento              esd ON esd.esdid  = doc.esdid ";
        $join[] = " INNER JOIN seguranca.usuario                    usu ON atm.usucpf_inclusao = usu.usucpf ";
        
        $params = array('where' => $where, 'join'=>$join);
        
        return $params;
    }
    
    public function cadastrarDadosAtividade($param = array()) {
        if(empty($param)){
            $param = $_POST;
            if(empty($param)){
                return false;
            }
        }
        
        $data = $param['atmdtlimiteconclusao'];
        $data = explode('/', $data);
        $param['atmdtlimiteconclusao'] = $data[2].'-'.$data[1].'-'.$data[0];
        
        foreach ($param as $key => $value) {
            if(empty($value)){
                $param[$key] = 'NULL';
            }
        }
        
        $param['atmnome'] = addslashes($param['atmnome']);
        $param['atmnome'] = substr($param['atmnome'], 0, 254);
        
        $param['atmobs'] = addslashes($param['atmobs']);
        $param['atmobs'] = substr($param['atmobs'], 0, 499);
        
        $sqlInsert = " INSERT INTO obras2.atividademonitoramento ( 
                                                                   estuf, muncod, entid, obrid, tamid, samid, atmnome, atmobs, atmdtlimiteconclusao, atmstatusrepetenaoconcluido,
                                                                   atmqtddiasstatusrepeticao, usucpf_inclusao, atmdtinclusao, atmdtatualizacao, atmstatus
                                                                  ) 
                                                           VALUES ( '".$param['estuf']."',
                                                                    ".$param['muncod'].",
                                                                    ".$param['entid'].",
                                                                    ".$param['obrid'].",
                                                                    ".$param['tamid'].",
                                                                    ".$param['samid'].",
                                                                    '".$param['atmnome']."',
                                                                    '".$param['atmobs']."',
                                                                    '".$param['atmdtlimiteconclusao']."',
                                                                    'N',
                                                                    0,
                                                                    '".$_SESSION['usucpf']."',
                                                                    now(),
                                                                    NULL,
                                                                    'A'
                                                           )RETURNING atmid  ";
        
        try{
            $atmid = $this->pegaUm($sqlInsert);
            $this->commit();
        } catch (Exception $ex) {
            $this->rollback();
            $atmid = false;
        }
        
        return $atmid;
    }
    
    public function editarDadosAtividade($param = array()) {
        
        if(empty($param)){
            $param = $_POST;
            if(empty($param)){
                return false;
            }
        }
        
        $atmid = $param['atmid'];
        if(empty($atmid)){
            return false;
        }
        
        $data = $param['atmdtlimiteconclusao'];
        $data = explode('/', $data);
        $param['atmdtlimiteconclusao'] = $data[2].'-'.$data[1].'-'.$data[0];
        
        $param['atmnome'] = addslashes($param['atmnome']);
        $param['atmnome'] = substr($param['atmnome'], 0, 254);
        
        $param['atmobs'] = addslashes($param['atmobs']);
        $param['atmobs'] = substr($param['atmobs'], 0, 499);
        
        $dados_atividade['estuf']                = "'".$param['estuf']."'"; 
        $dados_atividade['muncod']               = "'".$param['muncod']."'"; 
        $dados_atividade['entid']                = $param['entid']; 
        $dados_atividade['obrid']                = $param['obrid']; 
        $dados_atividade['tamid']                = $param['tamid']; 
        $dados_atividade['samid']                = $param['samid']; 
        $dados_atividade['atmnome']              = "'".$param['atmnome']."'"; 
        $dados_atividade['atmobs']               = "'".$param['atmobs']."'"; 
        $dados_atividade['atmdtatualizacao']     = 'now()'; 
        $dados_atividade['atmdtlimiteconclusao'] = "'".$param['atmdtlimiteconclusao']."'"; 
        
        foreach ($dados_atividade as $key => $value) {
            if(empty($value)){
                $dados_atividade[$key] = 'NULL';
            }
        }
        
        $update_atividade_campos  = "";
        foreach ($dados_atividade as $key => $value) {
            $update_atividade_campos  .= $key." = ".$value.',';            
        }
        $update_atividade = " UPDATE obras2.atividademonitoramento SET ".substr($update_atividade_campos,0,-1)." WHERE atmid = ".$atmid;
        
        try{
            $this->executar($update_atividade);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $this->commit();
        
        return $atmid;
    }
    
    public function excluirAtividade($atmid) {
        $sql = "UPDATE obras2.atividademonitoramento SET atmstatus = 'I' WHERE atmid = ".$atmid;
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $sql = "UPDATE obras2.itemmonitoramento SET itmstatus = 'I' WHERE atmid = ".$atmid;
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $this->commit();
        return true;
    }
    
    public function cadastrarDocidAtividade($atmid) {
        if(empty($atmid)){ return false; }
        require_once APPRAIZ . 'includes/workflow.php';
        $docdsc = "Fluxo da Atividade de Monitoramento Especial das Obras do módulo Obras II - atmid: " . $atmid;
        $docid  = wf_cadastrarDocumento(TPID_ME_ATIVIDADE_MONITORAMENTO, $docdsc);
        return $docid;
    }
    
    public function atualizaDocidAtividade($atmid) {
        if(empty($atmid)){ return false; }        
        $docid = $this->cadastrarDocidAtividade($atmid);        
        if(empty($docid)){
            $this->rollback();
            return false;
        }else{
            $sql = 'UPDATE obras2.atividademonitoramento SET docid = '.$docid.' WHERE atmid = '.$atmid;
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
    
    public function getEstadoAtividade($atmid) {
        $campos         = 'esd.esdid, esd.esddsc';
        $param['atmid'] = $atmid;
        $sql            = $this->getDadosAtividade($param, 'sql', $campos);
        $estado         = $this->pegaLinha($sql); 
        return $estado;
    }
    
    public function verificaAtmid($atmid) {
        $atmid = trim($atmid);
        if(empty($atmid) || $atmid == ''){ return false; }
        $sql = " SELECT atmid FROM obras2.atividademonitoramento WHERE  atmid = ".$atmid." AND atmstatus = 'A' ";
        $atmid = $this->pegaUm($sql);
        return $atmid;
    }
    
    public function mostraDadosAtividadeNoTarefa($atmid) {
        $campos = "atm.atmnome,
                   est.estuf,
                   mun.mundescricao";
        $dados = $this->getDadosAtividade(array('atmid'=>$atmid), $tipo_retorno = 'array', $campos = null);
        if(!empty($dados)){
            $html = '<table style="width: 50%">
                        <tr>
                            <th>Atividade</th>
                            <th>UF</th>
                            <th>Município</th>
                        </tr>
                        <tr>
                            <td>'.$dados[0]['atmnome'].'</td>
                            <td>'.$dados[0]['estuf'].'</td>
                            <td>'.$dados[0]['mundescricao'].'</td>
                        </tr>
                    </table>';
        }else{
            $html = '<strong>Dados não encontrados.</strong>';
        }
        return $html;
        
    }
        
    public function verificaConclusaoAtividade($atmid) {
        $estado = $this->getEstadoAtividade($atmid);
        if($estado[0]['esdid'] == ESDID_ME_ATIVIDADE_CONCLUIDO){
            return true;
        }
        return false;
    }
    
    public function posAcoesAtividadeConcluida($atmid) { }
    
    /**
     * 
     * MÉTODOS DAS TAREFAS (DOS ITENS) DE MONITORAMENTO
     * 
     */    
    public function montaListaTarefasMonitoramento($params = array(), $tipo = 'html'/*xls, minhasTarefas*/, $coluna_acoes = true ) {
        
        $cabecalho = array( 'Ações','Nome da Tarefa' , 'Observações da Tarefa', 'Data Limite', 'Repetir a Tarefa ?', 'Prazo para repetição', 'Arquivo',
                            'Responsável pela Tarefa', 'Responsável pelo Cadastro', 'Origem da Atividade', 'Obra','Entidade','Situação do Tarefa' );
        
        if(possuiPerfil(array(PFLCOD_SUPER_USUARIO)) && $tipo == 'html'){
            $acoes  = ' 
                        \'<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="javascript: alterarTarefa(\'|| itm.itmid ||\');" title="Alterar">
                          <img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer; margin-left: 3px;" onclick="javascript: excluirTarefa(\'|| itm.itmid ||\');" title="Excluir">\'
                         as acoes,
                      ';
        }else{
            $acoes  = ' \'<img align="absmiddle" src="/imagens/consultar.gif" style="cursor: pointer" onclick="javascript: visualizarTarefa(\'|| itm.itmid ||\');" title="Ver Tarefa">\' as acoes, ';
        }
        
        if($tipo == 'xls'){
            $acoes  = " '' as acoes, ";
        }
        
        if($tipo == 'minhasTarefas'){
            $acoes  = ' \'<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="javascript: alterarTarefa(\'|| itm.itmid ||\');" title="Alterar">
                          <img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer; margin-left: 3px;" onclick="javascript: excluirTarefa(\'|| itm.itmid ||\');" title="Excluir">
                          <img align="absmiddle" src="/imagens/editar_nome.gif" style="cursor: pointer" onclick="javascript: mostraEspelhoTarefa(\'|| itm.itmid ||\');" title="Analisar Tarefa">\' as acoes, ';
        }
        
        if($coluna_acoes == false){
            $acoes  = " '' as acoes, ";
        }
        
        $campos = " {$acoes}
                    itm.itmnome,
                    itm.itmobs,
                    COALESCE(TO_CHAR(itm.itmdtlimiteconclusao, 'dd/mm/YYYY')) AS itmdtlimiteconclusao,
                    CASE WHEN itm.itmstatusrepetenaoconcluido = 'N' THEN 'Não' ELSE 'Sim' END AS itmstatusrepetenaoconcluido,
                    itm.itmqtddiasstatusrepeticao,
                    CASE WHEN itm.arqidanexo IS NOT NULL 
                    THEN 
                        '<span onclick=\"download(' || itm.arqidanexo|| ')\" style=\"cursor: pointer vertical-align:middle\" title=\"Baixar Arquivo\">' ||
                        ' <img align=\"absmiddle\" src=\"/imagens/icone_lupa.png\" style=\"cursor: pointer vertical-align:middle\" title=\"Baixar Arquivo\"> ' ||
                          itm.itmnomeanexo ||
                        '</span>'
                    ELSE
                        'Não possui'
                    END AS arquivo,
                    usu_resp.usunome as resp_tarefa,
                    usu_add.usunome as resp_cad,
                    atm.atmnome,
                    '(' || obr.obrid || ')' || obr.obrnome as obra,
                    '(' || atm.entid || ')' || ent.entnome as entidade,
                    esd.esddsc 
                  ";
        
        $sql = $this->getDadosTarefas($params, 'sql', $campos);
        
        if ($tipo == 'xls') {            
            ob_clean();
            ini_set("memory_limit", "512M");
            header("Content-type: application/excel; name=ListaDeTarefasDeMonitoramentoEspecial.xls");
            header("Content-Disposition: attachment; filename=ListaDeTarefasDeMonitoramentoEspecial.xls");
            $this->sql_to_xml_excel($sql, 'ListaDeTarefasDeMonitoramentoEspecial', $cabecalho);
        } else {
            $this->monta_lista($sql, $cabecalho, 30, 10, 'N', 'center', 'N', 'N');
        }
    }
    
    public function getDadosTarefas($param = array(), $tipo_retorno = 'sql'/*array*/, $campos = null){
        
        $param = $this->getFiltrosJoinsDadosTarefasMonitoramento($param);
        $where = $param['where'];
        $join  = $param['join'];
        
        array_unshift($where, " itm.itmstatus = 'A' ");
        
        $strCampos = empty($campos) ? ' * ' : $campos;
        $strWhere  = implode(' AND ', $where);
        $strJoin   = implode('  ', $join);
        
        $sql = "SELECT {$strCampos} 
                FROM obras2.itemmonitoramento itm
                {$strJoin}
                WHERE {$strWhere}
                ";
            
        if($tipo_retorno == 'sql'){
            return $sql;
        }else{
            $arrDados = $this->carregar($sql);
            $arrDados = !empty($arrDados) ? $arrDados : array() ;
            return $arrDados;
        }
    }
    
    public function getFiltrosJoinsDadosTarefasMonitoramento($param) {
        if(empty($param)){
            $params = array('where' => array(), 'join'=>array());
            return $params;
        }
        
        $where = array();
        $join  = array();
        
        foreach ($param as $key => $value) {
            $value = is_string($value) ? trim($value) : $value;
            switch (true) {
                case ($key == 'itmid' && !empty($value)):
                    $where[] = " itm.itmid = ".$value;
                    break;
                case ($key == 'atmid' && !empty($value)):
                    $where[] = " itm.atmid = ".$value;
                    break;
                case ($key == 'usucpf_inclusao' && !empty($value)):
                    if (is_array($value)) {
                        foreach ($value as $k => $v) {
                            $value[$k] = "'{$v}'";
                        }
                    } else {
                        $value = array("'{$value}'");
                    }
                    foreach ($value as $k => $v) {
                        if (trim($v) == '' || $v == "''" || empty($v)) {
                            unset($value[$k]);
                        }
                    }
                    if (!empty($value)) {
                        $where[] = " itm.usucpf_inclusao IN ( " . implode(",", $value) . " ) ";
                    }
                    break;
                case ($key == 'usucpf_responsavel' && !empty($value)):
                    if (is_array($value)) {
                        foreach ($value as $k => $v) {
                            $value[$k] = "'{$v}'";
                        }
                    } else {
                        $value = array("'{$value}'");
                    }
                    foreach ($value as $k => $v) {
                        if (trim($v) == '' || $v == "''" || empty($v)) {
                            unset($value[$k]);
                        }
                    }
                    if (!empty($value)) {
                        $where[] = " resp.usucpf_responsavel IN ( " . implode(",", $value) . " ) ";
                    }
                    break;
                case ($key == 'esdid' && !empty($value)): 
                    $where[] = " esd.esdid = ".$value;
                    break;
                case ($key == 'itmcoletaranexostatus' && !empty($value) && $value != 'T'): 
                    $where[] = " itm.itmcoletaranexostatus = '".$value."'";
                    break;
                case ($key == 'itmnomeanexo' && !empty($value)): 
                    $where[] = " itm.itmnomeanexo ILIKE '%".$value."%'";
                    break;
                case ($key == 'itmstatusrepetenaoconcluido' && !empty($value) && $value != 'T'): 
                    $where[] = " itm.itmstatusrepetenaoconcluido = '".$value."'";
                    break;
                case ($key == 'itmnome' && !empty($value)): 
                    $valueTemp = removeAcentos(str_replace("-", " ", (trim($value))));
                    $where[]   = " ( ( UPPER(public.removeacento(itm.itmnome) ) ) ILIKE ('%" . $valueTemp . "%') OR itm.itmnome::CHARACTER VARYING ILIKE ('%" . $value . "%') ) ";
                    break;
                default :
                    break;
            }
        }
        
        if(!empty($param['itmqtddiasstatusrepeticao_especificado'])){
            $where[] = " itm.itmqtddiasstatusrepeticao = ".$param['itmqtddiasstatusrepeticao_especificado'];
        }else{
            if(!empty($param['itmqtddiasstatusrepeticao']) && $param['itmqtddiasstatusrepeticao'] != 'T' ){
                $where[] = " itm.itmqtddiasstatusrepeticao = ".$param['itmqtddiasstatusrepeticao'];
            }
        }
        
        if(!empty($param['itmdtinclusao_de']) && !empty($param['itmdtinclusao_ate'])){
            $de  = explode('/', $param['itmdtinclusao_de']);
            $ate = explode('/', $param['itmdtinclusao_ate']);
            $param['itmdtinclusao_de']  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $param['itmdtinclusao_ate'] = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " itm.itmdtinclusao BETWEEN '{$param['itmdtinclusao_de']}' AND '{$param['itmdtinclusao_ate']}' ";
        }elseif(!empty($param['itmdtinclusao_de']) && empty($param['itmdtinclusao_ate'])){
            $de  = explode('/', $param['itmdtinclusao_de']);
            $param['itmdtinclusao_de']  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " itm.itmdtinclusao = '{$param['itmdtinclusao_de']}' ";
        }elseif(empty($param['itmdtinclusao_de']) && !empty($param['itmdtinclusao_ate'])){
            $ate = explode('/', $param['itmdtinclusao_ate']);
            $param['itmdtinclusao_ate'] = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " itm.itmdtinclusao = '{$param['itmdtinclusao_ate']}' ";
        }
        
        if(!empty($param['itmdtlimiteconclusao_de']) && !empty($param['itmdtlimiteconclusao_ate'])){
            $de  = explode('/', $param['itmdtlimiteconclusao_de']);
            $ate = explode('/', $param['itmdtlimiteconclusao_ate']);
            $param['itmdtlimiteconclusao_de']  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $param['itmdtlimiteconclusao_ate'] = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " itm.itmdtlimiteconclusao BETWEEN '{$param['itmdtlimiteconclusao_de']}' AND '{$param['itmdtlimiteconclusao_ate']}' ";
        }elseif(!empty($param['itmdtlimiteconclusao_de']) && empty($param['itmdtlimiteconclusao_ate'])){
            $de  = explode('/', $param['itmdtlimiteconclusao_de']);
            $param['itmdtlimiteconclusao_de']  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " itm.itmdtlimiteconclusao = '{$param['itmdtlimiteconclusao_de']}' ";
        }elseif(empty($param['itmdtlimiteconclusao_de']) && !empty($param['itmdtlimiteconclusao_ate'])){
            $ate = explode('/', $param['itmdtlimiteconclusao_ate']);
            $param['itmdtlimiteconclusao_ate'] = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " itm.itmdtlimiteconclusao = '{$param['itmdtlimiteconclusao_ate']}' ";
        }
        
        $join[]   = " INNER JOIN obras2.atividademonitoramento           atm ON atm.atmid       = itm.atmid ";
        $join[]   = " LEFT  JOIN obras2.obras                            obr ON obr.obrid       = atm.obrid ";
        $join[]   = " INNER JOIN entidade.entidade                       ent ON atm.entid       = ent.entid ";
        $join[]   = " INNER JOIN seguranca.usuario                   usu_add ON usu_add.usucpf  = itm.usucpf_inclusao ";
        $join[]   = " INNER JOIN obras2.responsavelitemmonitoramento    resp ON itm.itmid       = resp.itmid AND resp.rimstatus = 'A' ";
        $join[]   = " INNER JOIN seguranca.usuario                  usu_resp ON usu_resp.usucpf = resp.usucpf_responsavel ";
        $join[]   = " INNER JOIN entidade.entidade              usu_resp_ent ON usu_resp.usucpf = usu_resp_ent.entnumcpfcnpj ";
        $join[]   = " LEFT  JOIN arquivo                                 arq ON arq.arqid       = itm.arqidanexo ";
        $join[]   = " INNER JOIN workflow.documento 	                 doc ON doc.docid       = itm.docid /*AND doc.tpdid = " .TPID_ME_ITEM_MONITORAMENTO."*/ ";
        $join[]   = " INNER JOIN workflow.estadodocumento                esd ON esd.esdid       = doc.esdid ";
        
        $params = array('where' => $where, 'join'=>$join);
        
        return $params;
    }
    
    public function cadastrarDadosTarefas($param = array()) {
        
        if(empty($param)){
            $param = $_POST;
            if(empty($param)){
                return false;
            }
        }
        
        $data = $param['itmdtlimiteconclusao'];
        $data = explode('/', $data);
        $param['itmdtlimiteconclusao'] = $data[2].'-'.$data[1].'-'.$data[0];
        
        foreach ($param as $key => $value) {
            if(empty($value)){
                $param[$key] = NULL;
            }
        }
        
        $param['itmnome'] = addslashes($param['itmnome']);
        $param['itmnome'] = substr($param['itmnome'], 0, 254);
        $param['itmobs']  = addslashes($param['itmobs']);
        $param['itmobs']  = substr($param['itmobs'], 0, 499);
        
        $param['itmqtddiasstatusrepeticao'] = (empty($param['itmqtddiasstatusrepeticao']) || $param['itmqtddiasstatusrepeticao'] == NULL) ? trim($param['itmqtddiasstatusrepeticao_especificado']) : $param['itmqtddiasstatusrepeticao'];
        
        $sqlInsert = " INSERT INTO obras2.itemmonitoramento ( atmid, itmnome, itmobs, itmcoletaranexostatus, itmnomeanexo, itmdtlimiteconclusao, itmstatusrepetenaoconcluido,
                                                              itmqtddiasstatusrepeticao, usucpf_inclusao, itmdtinclusao, itmstatus
                                                            ) 
                                                     VALUES ( '".$param['atmid']."',
                                                              '".$param['itmnome']."',
                                                              '".$param['itmobs']."',
                                                              '".$param['itmcoletaranexostatus']."',
                                                              '".$param['itmnomeanexo']."',
                                                              '".$param['itmdtlimiteconclusao']."',
                                                              '".$param['itmstatusrepetenaoconcluido']."',
                                                              ".$param['itmqtddiasstatusrepeticao'].",
                                                              '".$_SESSION['usucpf']."',
                                                              now(),
                                                              'A'
                                                     )RETURNING itmid  ";
        
        try{
            $itmid = $this->pegaUm($sqlInsert);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        
        $rimid = $this->atualizaDadosUsuarioResponsavelTarefa($param, $itmid);
        if(empty($rimid)){
            return false;
        }
        
        if($_FILES["arqidanexo"]['size'] != 0 && $_FILES["arqidanexo"]['size'] > 0){
            $resp = $this->uploadArquivoTarefaMonitoramento($itmid, $_POST);
            $msg = $resp['msg'];
        }
        
        $this->commit();
        return $itmid;
    }
    
    public function editarDadosTarefa($param = array()) {
        
        if(empty($param)){
            $param = $_POST;
            if(empty($param)){
                return false;
            }
        }
        
        $itmid = $param['itmid'];
        if(empty($itmid)){
            return false;
        }
        
        if(empty($param['atmid'])){
            return false;
        }
        
        $data = $param['itmdtlimiteconclusao'];
        $data = explode('/', $data);
        $param['itmdtlimiteconclusao'] = $data[2].'-'.$data[1].'-'.$data[0];
        
        $param['itmnome'] = addslashes($param['itmnome']);
        $param['itmnome'] = substr($param['itmnome'], 0, 254);
        
        $param['itmobs'] = addslashes($param['itmobs']);
        $param['itmobs'] = substr($param['itmobs'], 0, 499);
        
        $dados_tarefa['atmid']                       = $param['atmid'];
        $dados_tarefa['itmnome']                     = "'".$param['itmnome']."'";
        $dados_tarefa['itmcoletaranexostatus']       = "'".$param['itmcoletaranexostatus']."'";
        $dados_tarefa['itmnomeanexo']                = "'".$param['itmnomeanexo']."'";
        $dados_tarefa['itmstatusrepetenaoconcluido'] = "'".$param['itmstatusrepetenaoconcluido']."'";
        $dados_tarefa['itmobs']                      = "'".$param['itmobs']."'"; 
        $dados_tarefa['itmdtlimiteconclusao']        = "'".$param['itmdtlimiteconclusao']."'";
        $dados_tarefa['itmdtatualizacao']            = 'now()'; 
        
        if(empty($param['itmqtddiasstatusrepeticao'])){
            $param['itmqtddiasstatusrepeticao_especificado'] = trim($param['itmqtddiasstatusrepeticao_especificado']);
            if(!empty($param['itmqtddiasstatusrepeticao_especificado'])){
                $dados_tarefa['itmqtddiasstatusrepeticao'] = $param['itmqtddiasstatusrepeticao_especificado']; // 0
            }else{
                $dados_tarefa['itmqtddiasstatusrepeticao'] =  0;
            }
        }else{
            $dados_tarefa['itmqtddiasstatusrepeticao'] = $param['itmqtddiasstatusrepeticao'];
        }
        
        foreach ($dados_tarefa as $key => $value) {
            if(empty($value)){
                $dados_tarefa[$key] = 'NULL';
            }
        }
        
        $update_tarefa_campos  = "";
        foreach ($dados_tarefa as $key => $value) {
            $update_tarefa_campos  .= $key." = ".$value.',';            
        }
        $update_tarefa = " UPDATE obras2.itemmonitoramento SET ".substr($update_tarefa_campos,0,-1)." WHERE itmid = ".$itmid;
        
        try{
            $this->executar($update_tarefa);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        
        $rimid = $this->atualizaDadosUsuarioResponsavelTarefa($param);
        if(empty($rimid)){
            return false;
        }
        
        if($_FILES["arqidanexo"]['size'] != 0 && $_FILES["arqidanexo"]['size'] > 0){
            $resp = $this->uploadArquivoTarefaMonitoramento($itmid, $_POST);
            $msg = $resp['msg'];
            if($resp['erro'] == true){
                $this->rollback();
                return false;
            }
        }
        
        $this->commit();
        return $itmid;
    }
    
    public function atualizaDadosUsuarioResponsavelTarefa($param, $itmid = null) {
        
        if(empty($itmid)){ $itmid = $param['itmid']; }
        
        if(empty($itmid)){ return false; }
        
        $sqlUsucpf = "SELECT entnumcpfcnpj FROM entidade.entidade WHERE  entid = ".$param['entidresponsavel'];
        $usucpf_enviado    = $this->pegaUm($sqlUsucpf);
        
        $sqlUsucpf = "SELECT usucpf_responsavel FROM obras2.responsavelitemmonitoramento WHERE  itmid = ".$itmid." AND rimstatus = 'A' ";
        $usucpf_cadastrado = $this->pegaUm($sqlUsucpf);
        
        $sqlRimid = "SELECT rimid FROM obras2.responsavelitemmonitoramento WHERE  itmid = ".$itmid." AND rimstatus = 'A' ";
        $rimidCadastrado = $this->pegaUm($sqlUsucpf);
        
        if($usucpf_enviado != $usucpf_cadastrado){
            if(!empty($usucpf_cadastrado)){
                //Atualiza usuário responsável
                $sqlUpdateUsu = 'UPDATE obras2.responsavelitemmonitoramento SET rimstatus = \'I\' WHERE itmid = '.$itmid;
                try{
                    $this->executar($sqlUpdateUsu);
                }catch (Exception $e){
                    $this->rollback();
                    return false;
                }
            }
            
            if(!empty($usucpf_enviado)){
                $sqlInsertUsu = " INSERT INTO obras2.responsavelitemmonitoramento ( itmid, usucpf_responsavel, usucpf_inclusao, rimdtinclusao, rimstatus ) 
                                                                           VALUES ( ".$itmid.", '".$usucpf_enviado."', '".$_SESSION['usucpf']."', now(), 'A') RETURNING rimid; ";

                try{
                    $rimid = $this->pegaUm($sqlInsertUsu);
                } catch (Exception $ex) {
                    $this->rollback();
                    return false;
                }
            }
        }else{
            if(empty($usucpf_cadastrado)){
                $sqlInsertUsu = " INSERT INTO obras2.responsavelitemmonitoramento ( itmid, usucpf_responsavel, usucpf_inclusao, rimdtinclusao, rimstatus ) 
                                                                           VALUES ( ".$itmid.", '".$usucpf_enviado."', '".$_SESSION['usucpf']."', now(), 'A') RETURNING rimid; ";

                try{
                    $rimid = $this->pegaUm($sqlInsertUsu);
                } catch (Exception $ex) {
                    $this->rollback();
                    return false;
                }
            }
        }
        
        $rimid = empty($rimid) ? $rimidCadastrado : $rimid;
        return $rimid;
    }
    
    public function uploadArquivoTarefaMonitoramento($itmid, $post = array()){
        
        if($_FILES['arqidanexo']['size'] == 0){
            return array('erro'=>false, 'msg'=>'Arquivo não enviado.');
        }
        
        $erro     = false;
        $msg_erro = '';
        
        if(empty($post)){ $post = $_POST; }
        
        $arquivo          = $_FILES['arqidanexo'];
        $arquivosparts    = explode(".", $arquivo["name"]);
        $arqnome          = current($arquivosparts);
        $arqextensao      = end($arquivosparts);
        $descricaoArquivo = $post['itmnomeanexo'];

        $sql = "INSERT INTO public.arquivo (arqnome,
                                            arqextensao,
                                            arqdescricao,
                                            arqtipo,
                                            arqtamanho,
                                            arqdata,
                                            arqhora,
                                            usucpf,
                                            sisid,
                                            arqstatus)
                        VALUES( '".$arqnome."',
                                '".$arqextensao."',
                                '".$descricaoArquivo."',
                                '".$arquivo["type"]."',
                                '".$arquivo["size"]."',
                                '".date('Y-m-d')."',
                                '".date('H:i:s')."',
                                '".$_SESSION["usucpf"]."',
                                 ".$_SESSION["sisid"].",
                                'A') 
                        RETURNING arqid;";
        try{
            $arqid = $this->pegaUm($sql);
        } catch (Exception $ex) {
            $arqid     = false;
            $erro      = true;
            $msg_erro .= 'Erro ao cadastrar o arquivo público.'.$ex->getMessage();
        }

        if($arqid){
            $nomeEsquema = 'obras2';
            $caminhoArquivo = APPRAIZ."arquivos/".$nomeEsquema."/".floor($arqid/1000).'/'.$arqid;

            if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema)) {
                    mkdir(APPRAIZ."arquivos/".$nomeEsquema, 0777);
            }
            if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000))) {
                $pasta = APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000);
                    $resp = mkdir($pasta, 0777, true);
            }

            switch($arquivo["type"]) {
                case 'image/jpeg':
                    ini_set("memory_limit", "128M");
                    list($width, $height) = getimagesize($arquivo["tmp_name"]);
                    $original_x = $width;
                    $original_y = $height;
                    // se a largura for maior que altura
                    if($original_x > $original_y) {
                        if($original_x != 0){
                            $porcentagem = (100 * 640) / $original_x;
                        }else{
                            $porcentagem = 0;
                        }
                    }else {
                        if($original_y != 0){
                            $porcentagem = (100 * 480) / $original_y;  
                        }else{
                            $porcentagem = 0;
                        }
                    }

                    $tamanho_x = $original_x * ($porcentagem / 100);
                    $tamanho_y = $original_y * ($porcentagem / 100);
                    $image_p   = imagecreatetruecolor($tamanho_x, $tamanho_y);
                    $image     = imagecreatefromjpeg($arquivo['tmp_name']);

                    imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
                    imagejpeg($image_p, $caminhoArquivo, 100);
                    ImageDestroy($image_p);//Clean-up memory
                    ImageDestroy($image);//Clean-up memory

                    break;
                default:
                    if ( !move_uploaded_file( $arquivo["tmp_name"], $caminhoArquivo) ) {
                        $erro      = true;
                        $msg_erro .= ' \n Problemas no envio do arquivo '.$c.' da lista.';
                    }
            }

            $sql = " UPDATE obras2.itemmonitoramento SET arqidanexo = ".$arqid." WHERE itmid = ".$itmid.";";

            try{
                $rarid = $this->pegaUm($sql);
            } catch (Exception $ex) {
                $rarid     = false;
                $erro      = true;
                $msg_erro .= 'Erro ao cadastrar o arquivo do Tarefa de Monitoramento.'.$ex->getMessage();
            }
        }
        
        if($erro){
            $this->rollback();
            $msg = $msg_erro;
        }else{
            $this->commit();
            $msg = ' Arquivos cadastrados com sucesso. ';
        }
        
        return array('erro'=>$erro, 'msg'=>$msg);

    }
    
    public function atualizaResponsavelTarefa($itmid, $usucpf) {
        $sql = "UPDATE obras2.responsavelitemmonitoramento SET rimstatus = 'I' WHERE itmid = ".$itmid;
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        
        $sql = "INSERT INTO obras2.responsavelitemmonitoramento (itmid, usucpf_responsavel, usucpf_inclusao, rimdtinclusao, rimstatus)
                                                          VALUES(".$itmid.", '".$usucpf."', '".$_SESSION['usucpf']."', NOW(), 'A') RETURNING rimid; ";
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        
        $this->commit();
        return true;
    }
    
    public function excluirTarefa($itmid) {
        $sql = "UPDATE obras2.itemmonitoramento SET itmstatus = 'I' WHERE itmid = ".$itmid;
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $this->commit();
        return true;
    }
    
    public function cadastrarDocidTarefa($itmid) {
        if(empty($itmid)){ return false; }
        require_once APPRAIZ . 'includes/workflow.php';
        $docdsc = "Fluxo dos Tarefas de Monitoramento Especial das Obras do módulo Obras II - itmid: " . $itmid;
        $docid  = wf_cadastrarDocumento(TPID_ME_ITEM_MONITORAMENTO, $docdsc);
        return $docid;
    }
    
    public function atualizaDocidTarefa($itmid) {
        if(empty($itmid)){ return false; }        
        $docid = $this->cadastrarDocidTarefa($itmid);        
        if(empty($docid)){
            $this->rollback();
            return false;
        }else{
            $sql = 'UPDATE obras2.itemmonitoramento SET docid = '.$docid.' WHERE itmid = '.$itmid;
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
    
    public function getEstadoTarefa($itmid) {
        $campos         = 'ed.esdid, ed.esddsc';
        $param['itmid'] = $itmid;
        $sql            = $this->getDadosTarefa($param, 'sql', $campos);
        $estado         = $this->pegaLinha($sql); 
        return $estado;
    }
    
    public function downloadArquivoTarefa($arqid = null) {
        if(empty($arqid)){ $arqid = $_REQUEST['arqid']; }
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec(null,null,'obras2');
	$file->getDownloadArquivo( $arqid );
	die('<script type="text/javascript">
                document.location.href = document.location.href;
             </script>');
    }
    
    public function getHistoricoResponsaveis($itmid){
        if(empty($itmid)){
            return array();
        }
        
        $html = '';
        
        $sqlUsucpf = " SELECT ur.usucpf_responsavel, 
                              su.usunome,
                              CASE WHEN ur.rimstatus = 'I' 
                              THEN
                                'Inativo'
                              ELSE
                                'Ativo'
                              END as situacao
                       FROM obras2.responsavelitemmonitoramento ur
                       INNER JOIN seguranca.usuario su ON ur.usucpf_responsavel = su.usucpf
                       WHERE  ur.itmid = ".$itmid." 
                       ORDER BY ur.rimstatus, su.usunome ASC
                       " ;
        $dados = $this->carregar($sqlUsucpf);
        
        if($dados){
            
                $html = '<table style="width: 50%">
                            <tr>
                                <th>CPF</th>
                                <th>Nome</th>
                                <th>Situação</th>
                            </tr>';
            foreach ($dados as $key => $value) {
                $html .= '  <tr>
                                <td>'.$dados[$key]['usucpf_responsavel'].'</td>
                                <td>'.$dados[$key]['usunome'].'</td>
                                <td>'.$dados[$key]['situacao'].'</td>
                            </tr>
                        ';
            }
            $html .= '</table>';
        }else{
            $html = '<strong>Dados não encontrados.</strong>';
        }
        return $html;
    }
    
    public function posAcoesTarefaConcluida($itmid, $atmid) {
        
        /**
         * - Quando o usuário concluir o item mas sem finalizar a atividade o sistema vai gerar automaticamente uma copia do item com a 
         *   data de conclusao pevista da atividade + o numero de dias pra repetir de forma q ele tenha q fazer de novo
         */
        if(!$this->verificaConclusaoAtividade($atmid)){
            $clonou = $this->clonarTarefa($itmid, $atmid);
            if(!$clonou){
                echo '<script > 
                        alert(\'Erro ao Clonar a Tarefa.\');
                      </script>';
            }
        }
        
    }
    
    public function clonarTarefa($itmid, $atmid = null, $atmid_duplicada = false) {
        
        //Recupera os dados da Tarefa
        //Recupera os dados do Responsavel pela Tarefa
        //Recupera a data da Atividade caso $atmid != null
        //Cadastra dos dados da Tarefa
        //Cadastra os dados do Responsável
        
        $dadosTarefa          = $this->getDadosTarefas(array('itmid'=>$itmid), 'array', ' itm.* ');
        $dados_tarefa         = $dadosTarefa[0];
        $dadosRespTarefa      = $this->getDadosTarefas(array('itmid'=>$itmid), 'array', ' resp.* ');
        $dados_resp_tarefa    = $dadosRespTarefa[0];

        $dados_tarefa['itmqtddiasstatusrepeticao'] = !empty($dados_tarefa['itmqtddiasstatusrepeticao']) ? $dados_tarefa['itmqtddiasstatusrepeticao'] : 1;
        $qtdAddDias = $dados_tarefa['itmqtddiasstatusrepeticao'];
        
        if(empty($atmid_duplicada)){
            if(!empty($atmid)){
                $dtConclusao = $this->pegaUm('SELECT atm.atmdtlimiteconclusao FROM obras2.atividademonitoramento atm WHERE atm.atmid = '.$atmid);
            }else{
                $dtConclusao = $dados_tarefa['itmdtlimiteconclusao'];
            }
            $data = date('Y-m-d', strtotime($dtConclusao. ' + '.$qtdAddDias.' days'));
            $dados_tarefa['itmdtlimiteconclusao'] = $data;
        }else{
            $dados_tarefa['atmid'] = $atmid_duplicada;
        }
        
        $sqlInsert = " INSERT INTO obras2.itemmonitoramento ( atmid, itmnome, itmobs, itmcoletaranexostatus, itmnomeanexo, itmdtlimiteconclusao, itmstatusrepetenaoconcluido,
                                                                  itmqtddiasstatusrepeticao, usucpf_inclusao, itmdtinclusao, itmstatus
                                                                ) 
                                                         VALUES ( '".$dados_tarefa['atmid']."',
                                                                  '".$dados_tarefa['itmnome']."',
                                                                  '".$dados_tarefa['itmobs']."',
                                                                  '".$dados_tarefa['itmcoletaranexostatus']."',
                                                                  '".$dados_tarefa['itmnomeanexo']."',
                                                                  '".$dados_tarefa['itmdtlimiteconclusao']."',
                                                                  '".$dados_tarefa['itmstatusrepetenaoconcluido']."',
                                                                  ".$dados_tarefa['itmqtddiasstatusrepeticao'].",
                                                                  '".$_SESSION['usucpf']."',
                                                                  now(),
                                                                  'A'
                                                         )RETURNING itmid;  ";

        try{
            $itmid = $this->pegaUm($sqlInsert);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }

        $sqlInsertUsu = " INSERT INTO obras2.responsavelitemmonitoramento ( itmid, usucpf_responsavel, usucpf_inclusao, rimdtinclusao, rimstatus ) 
                                                                   VALUES ( ".$itmid.", '".$dados_resp_tarefa['usucpf_responsavel']."', '".$_SESSION['usucpf']."', now(), 'A') RETURNING rimid; ";

        try{
            $rimid = $this->pegaUm($sqlInsertUsu);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        
        $this->atualizaDocidTarefa($itmid);
        $this->commit();
        return true;
        
    }
    
    /**
     * 
     * MÉTODOS DE ANÁLISE DAS TAREFAS (DOS ITENS) DE MONITORAMENTO
     * 
     */    
    public function getDadosAnaliseTarefa($itmid) {        
        if(empty($itmid)){ return false; }        
        $sql = " SELECT anm.*, 
                        arq.arqnome || '.' || arqextensao as arqnome
                 FROM obras2.analisemonitoramento  anm
                 LEFT JOIN public.arquivo arq ON anm.arqidanexo = arq.arqid
                 WHERE anm.anmstatus = 'A'
                   AND anm.itmid = ".$itmid;
        $arrDados = $this->carregar($sql);
        $arrDados = !empty($arrDados) ? $arrDados : array() ;
        return $arrDados;
    }
    
    public function cadastrarDadosAnaliseTarefa($params) {        
        $str_anmnome = '';        
        if(!empty($params['itmid'])){
            $str_anmnome = ' da Tarefa de ID: '.$params['itmid'];
        }        
        if(!empty($params['atmid'])){
            $str_anmnome = ' da Atividade de ID: '.$params['atmid'];
        }        
        $dados['itmid']                = empty($params['itmid']) ? 'NULL' : $params['itmid'];
        $dados['atmid']                = empty($params['atmid']) ? 'NULL' : $params['atmid'];
        $dados['anmrespostaconclusao'] = "'".$params['anmrespostaconclusao']."'";
        $dados['anmnome']              = "'Dados da análise".$str_anmnome."'";
        $dados['anmobs']               = empty($params['anmobs']) ? "''" : "'".$params['anmobs']."'";
        $dados['usucpf_inclusao']      = "'".$_SESSION['usucpf']."'";
        $dados['anmdtinclusao']        = 'now()';
        $dados['anmstatus']            = "'A'";
        
        $sql = " INSERT INTO obras2.analisemonitoramento (itmid, atmid, anmrespostaconclusao, anmnome, anmobs, usucpf_inclusao, anmdtinclusao, anmstatus)
                                                  VALUES (".$dados['itmid'].","
                                                           .$dados['atmid'].","
                                                           .$dados['anmrespostaconclusao'].","
                                                           .$dados['anmnome'].","
                                                           .$dados['anmobs'].","
                                                           .$dados['usucpf_inclusao'].","
                                                           .$dados['anmdtinclusao'].","
                                                           .$dados['anmstatus']." ) RETURNING anmid; ";
        
        try{
            $anmid = $this->pegaUm($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        
        $resp = $this->uploadArquivoAnaliseTarefaMonitoramento($anmid);    
        if($resp['erro'] == true){
            echo '<script > 
                    alert(\''.$resp['msg'].'\');
                  </script>' ;
        }
        
        $this->commit();
        return $anmid;
        
    }
    
    public function editarDadosAnaliseTarefa($params) {
        
        $anmid = $params['anmid'];
        if(empty($anmid)){
            return false;
        }
        
        $dados['anmrespostaconclusao'] = "'".$params['anmrespostaconclusao']."'";
        $dados['anmobs']               = empty($params['anmobs']) ? "''" : "'".$params['anmobs']."'";
        
        $sql = " UPDATE obras2.analisemonitoramento 
                 SET anmrespostaconclusao = ".$dados['anmrespostaconclusao'].",
                     anmobs               = ".$dados['anmobs']." 
                 WHERE anmid =".$anmid.";";
        
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        
        $resp = $this->uploadArquivoAnaliseTarefaMonitoramento($anmid);        
        if($resp['erro'] == true){
            echo '<script > 
                    alert(\''.$resp['msg'].'\');
                  </script>' ;
        }
        
        $this->commit();
        return $anmid;
    }
    
    public function excluirDadosAnaliseTarefa($anmid) {
        if(empty($anmid)){
            return false;
        }
        $sql = "UPDATE obras2.analisemonitoramento SET anmstatus = 'I' WHERE anmid = ".$anmid;
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $this->commit();
        return true;
    }
    
    public function uploadArquivoAnaliseTarefaMonitoramento($anmid){
        
        if($_FILES['arqidanexo']['size'] == 0){
            return array('erro'=>false, 'msg'=>'Arquivo não enviado.');
        }
        
        $erro     = false;
        $msg_erro = '';
        
        $arquivo          = $_FILES['arqidanexo'];
        $arquivosparts    = explode(".", $arquivo["name"]);
        $arqnome          = current($arquivosparts);
        $arqextensao      = end($arquivosparts);
        $descricaoArquivo = 'Arquivo da Análise de Tarefa de Monitoramento Especial.';//$post['itmnomeanexo'];

        $sql = "INSERT INTO public.arquivo (arqnome,
                                            arqextensao,
                                            arqdescricao,
                                            arqtipo,
                                            arqtamanho,
                                            arqdata,
                                            arqhora,
                                            usucpf,
                                            sisid,
                                            arqstatus)
                        VALUES( '".$arqnome."',
                                '".$arqextensao."',
                                '".$descricaoArquivo."',
                                '".$arquivo["type"]."',
                                '".$arquivo["size"]."',
                                '".date('Y-m-d')."',
                                '".date('H:i:s')."',
                                '".$_SESSION["usucpf"]."',
                                 ".$_SESSION["sisid"].",
                                'A') 
                        RETURNING arqid;";
        try{
            $arqid = $this->pegaUm($sql);
        } catch (Exception $ex) {
            $arqid     = false;
            $erro      = true;
            $msg_erro .= 'Erro ao cadastrar o arquivo público.'.$ex->getMessage();
        }

        if($arqid){
            $nomeEsquema = 'obras2';
            $caminhoArquivo = APPRAIZ."arquivos/".$nomeEsquema."/".floor($arqid/1000).'/'.$arqid;

            if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema)) {
                    mkdir(APPRAIZ."arquivos/".$nomeEsquema, 0777);
            }
            if(!is_dir(APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000))) {
                $pasta = APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000);
                    $resp = mkdir($pasta, 0777, true);
            }

            switch($arquivo["type"]) {
                case 'image/jpeg':
                    ini_set("memory_limit", "128M");
                    list($width, $height) = getimagesize($arquivo["tmp_name"]);
                    $original_x = $width;
                    $original_y = $height;
                    // se a largura for maior que altura
                    if($original_x > $original_y) {
                        if($original_x != 0){
                            $porcentagem = (100 * 640) / $original_x;
                        }else{
                            $porcentagem = 0;
                        }
                    }else {
                        if($original_y != 0){
                            $porcentagem = (100 * 480) / $original_y;  
                        }else{
                            $porcentagem = 0;
                        }
                    }

                    $tamanho_x = $original_x * ($porcentagem / 100);
                    $tamanho_y = $original_y * ($porcentagem / 100);
                    $image_p   = imagecreatetruecolor($tamanho_x, $tamanho_y);
                    $image     = imagecreatefromjpeg($arquivo['tmp_name']);

                    imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
                    imagejpeg($image_p, $caminhoArquivo, 100);
                    ImageDestroy($image_p);//Clean-up memory
                    ImageDestroy($image);//Clean-up memory

                    break;
                default:
                    if ( !move_uploaded_file( $arquivo["tmp_name"], $caminhoArquivo) ) {
                        $erro      = true;
                        $msg_erro .= ' \n Problemas no envio do arquivo '.$c.' da lista.';
                    }
            }

            $sql = " UPDATE obras2.analisemonitoramento SET arqidanexo = ".$arqid." WHERE anmid = ".$anmid.";";

            try{
                $rarid = $this->pegaUm($sql);
            } catch (Exception $ex) {
                $rarid     = false;
                $erro      = true;
                $msg_erro .= 'Erro ao cadastrar o arquivo da Análise da Tarefa de Monitoramento.'.$ex->getMessage();
            }
        }
        
        if($erro){
            $this->rollback();
            $msg = $msg_erro;
        }else{
            $this->commit();
            $msg = ' Arquivos cadastrados com sucesso. ';
        }
        
        return array('erro'=>$erro, 'msg'=>$msg);

    }
    
    
    public function montaListaTipoAtividadesMonitoramento($params = array(), $tipo = 'html'/*xls*/, $coluna_acoes = true ) {
        $cabecalho = array( "Ações",'ID','Nome Tipo','Data de Inclusão');
        if(possuiPerfil(array(PFLCOD_SUPER_USUARIO))){
            $acoes  = ' 
                        \'<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="javascript: alterarTipoAtividade(\'|| tamid ||\');" title="Alterar">
                          <img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer; margin-left: 3px;" onclick="javascript: excluirTipoAtividade(\'|| tamid ||\');" title="Excluir">\'
                         as acoes,
                      ';
        }
        if($tipo == 'xls'){
            $acoes  = " '' as acoes, ";
        }
        if($coluna_acoes == false){
            $acoes  = " '' as acoes, ";
        }
        $campos = " {$acoes}
                    tamid,
                    tamdesc,
                    COALESCE(TO_CHAR(tamdtinclusao, 'dd/mm/YYYY')) AS tamdtinclusao
                  ";
        
        $where = $params;
        $strWhere  = implode(' AND ', $where);
        
        $sql = " SELECT ".$campos."  
                 FROM obras2.tipoatividademonitoramento 
                 WHERE tamstatus = 'A' 
                 {$strWhere}";
        
        if ($tipo == 'xls') {            
            ob_clean();
            ini_set("memory_limit", "512M");
            header("Content-type: application/excel; name=ListaDeTipoDeAtividadesDeMonitoramentoEspecial.xls");
            header("Content-Disposition: attachment; filename=ListaDeTipoDeAtividadesDeMonitoramentoEspecial.xls");
            $this->sql_to_xml_excel($sql, 'ListaDeTipoDeAtividadesDeMonitoramentoEspecial', $cabecalho);
        } else {
            $this->monta_lista($sql, $cabecalho, 30, 10, 'N', 'center', 'N', 'N');
        }
    }
    
    public function cadastrarDadosTipoAtividade($param){        
        if(empty($param)){
            $param = $_REQUEST;
            if(empty($param)){
                return false;
            }
        }        
        $param['tamdesc'] = addslashes($param['tamdesc']);        
        $sqlInsert = " INSERT INTO obras2.tipoatividademonitoramento ( tamdesc, tamstatus, tamdtinclusao ) VALUES ( '".$param['tamdesc']."', 'A', now() )RETURNING tamid  ";        
        try{
            $tamid = $this->pegaUm($sqlInsert);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }        
        $this->commit();
        return $tamid;
    }
    
    public function editarDadosTipoAtividade($param){
        if(empty($param)){
            $param = $_REQUEST;
            if(empty($param)){
                return false;
            }
        }
        $tamid = $param['tamid'];
        if(empty($tamid)){
            return false;
        }
        $param['tamdesc'] = addslashes($param['tamdesc']);
        $update_atividade = " UPDATE obras2.tipoatividademonitoramento SET tamdesc = '".$param['tamdesc']."' WHERE tamid = ".$tamid;
        try{
            $this->executar($update_atividade);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $this->commit();
        return $tamid;
    }
    
    public function excluirDadosTipoAtividade($tamid){
        $sql = "UPDATE obras2.tipoatividademonitoramento SET tamstatus = 'I' WHERE tamid = ".$tamid;
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $this->commit();
        return true;
    }
    
    public function montaListaSubtipoAtividadesMonitoramento($params = array(), $tipo = 'html'/*xls*/, $coluna_acoes = true ) {
        $cabecalho = array( "Ações",'ID','Nome Subtipo','Data de Inclusão');
        if(possuiPerfil(array(PFLCOD_SUPER_USUARIO))){
            $acoes  = ' 
                        \'<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer" onclick="javascript: alterarSubtipoAtividade(\'|| samid ||\');" title="Alterar">
                          <img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer; margin-left: 3px;" onclick="javascript: excluirSubtipoAtividade(\'|| samid ||\');" title="Excluir">\'
                         as acoes,
                      ';
        }
        if($tipo == 'xls'){
            $acoes  = " '' as acoes, ";
        }
        if($coluna_acoes == false){
            $acoes  = " '' as acoes, ";
        }
        $campos = " {$acoes}
                    samid,
                    samdesc,
                    COALESCE(TO_CHAR(samdtinclusao, 'dd/mm/YYYY')) AS samdtinclusao
                  ";
        
        $where = $params;
        $strWhere  = implode(' AND ', $where);
        
        $sql = " SELECT ".$campos."  
                 FROM obras2.subtipoatividademonitoramento 
                 WHERE samstatus = 'A' 
                 {$strWhere}";
        
        if ($tipo == 'xls') {            
            ob_clean();
            ini_set("memory_limit", "512M");
            header("Content-type: application/excel; name=ListaDeSubtipoDeAtividadesDeMonitoramentoEspecial.xls");
            header("Content-Disposition: attachment; filename=ListaDeSubtipoDeAtividadesDeMonitoramentoEspecial.xls");
            $this->sql_to_xml_excel($sql, 'ListaDeSubtipoDeAtividadesDeMonitoramentoEspecial', $cabecalho);
        } else {
            $this->monta_lista($sql, $cabecalho, 30, 10, 'N', 'center', 'N', 'N');
        }
    }
    
    
    public function cadastrarDadosSubtipoAtividade($param){        
        if(empty($param)){
            $param = $_REQUEST;
            if(empty($param)){
                return false;
            }
        }        
        $param['samdesc'] = addslashes($param['samdesc']);        
        $sqlInsert = " INSERT INTO obras2.subtipoatividademonitoramento ( samdesc, samstatus, samdtinclusao ) VALUES ( '".$param['samdesc']."', 'A', now() )RETURNING samid  ";        
        try{
            $samid = $this->pegaUm($sqlInsert);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }        
        $this->commit();
        return $samid;
    }
    
    public function editarDadosSubtipoAtividade($param){
        if(empty($param)){
            $param = $_REQUEST;
            if(empty($param)){
                return false;
            }
        }
        $samid = $param['samid'];
        if(empty($samid)){
            return false;
        }
        $param['samdesc'] = addslashes($param['samdesc']);
        $update_atividade = " UPDATE obras2.subtipoatividademonitoramento SET samdesc = '".$param['samdesc']."' WHERE samid = ".$samid;
        try{
            $this->executar($update_atividade);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $this->commit();
        return $samid;
    }
    
    public function excluirDadosSubtipoAtividade($samid){
        $sql = "UPDATE obras2.subtipoatividademonitoramento SET samstatus = 'I' WHERE samid = ".$samid;
        try{
            $this->executar($sql);
        } catch (Exception $ex) {
            $this->rollback();
            return false;
        }
        $this->commit();
        return true;
    }
    
    
}