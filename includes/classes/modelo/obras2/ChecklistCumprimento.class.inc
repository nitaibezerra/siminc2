<?php

class ChecklistCumprimento extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.checklistcumprimento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("ckfid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'ckfid' => null,
                                    'obrid' => null,
                                    'qrpid' => null,
                                    'docid' => null,
                                    'usucpf' => null,
                                    'ckfdatainclusao' => null,
                                    'ckfstatus' => null
                                  );

    /**
     * 
     * @param int $obrid
     * @explicacao do uso da referência (&$obrid) http://www.php.net/manual/pt_BR/language.references.arent.php
     */
    public function validaParametroObrid(&$obrid) {
        if (empty($obrid) || !is_numeric($obrid)) {
            $obrid = $this->obrid;
            if (empty($obrid)) {
                $obrid = $_SESSION['obras2']['obrid'];
                if (empty($obrid)) {
                    $obrid = false;
                }
            }
        }
        $obrid = (int) $obrid;
    }
    
    public function getFiltrosDadosListaChecklistGeral() {
        $where = array();
        
        extract($_REQUEST);
        
        if (!empty($obrbuscatexto)) {
            $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrbuscatexto))));
            $where[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                                        obr.obrid::CHARACTER VARYING ILIKE ('%" . $obrbuscatexto . "%') ) ";
        }

        if (!empty($tpoid)) {
            $where[] = " obr.tpoid = $tpoid ";
        }

        if (!empty($item_checklist)) {            
            switch ($item_checklist) {
                case '2P':
                    $where[] = " que.queid = '".QUEID_QUEST_CHKLST_2P."' ";
                    break;
                case 'AD':
                    $where[] = " que.queid = '".QUEID_QUEST_CHKLST_ADM."' ";
                    break;
                case 'TC':
                    $where[] = " que.queid = '".QUEID_QUEST_CHKLST_TEC."' ";
                    break;
                case 'OV':
                    $where[] = " que.queid = '".QUEID_QUEST_CHKLST_OBR_VINC."' ";
                    break;
                default:
                    $where[] = " que.queid IN (".QUEID_QUEST_CHKLST_2P.", ".QUEID_QUEST_CHKLST_ADM.", ".QUEID_QUEST_CHKLST_TEC.", ".QUEID_QUEST_CHKLST_OBR_VINC.") ";
                    break;
            }
        }

        if (!empty($esdid_ck) && is_array($esdid_ck) && $esdid_ck[0] != '') {
            $where[] = " esd_ck.esdid IN ( " . implode(",", $esdid_ck) . " ) ";
        }
        
        if (!empty($usucpf)) {
            if (is_array($usucpf)) {
                foreach ($usucpf as $k => $v) {
                    $usucpf[$k] = "'{$v}'";
                }
            } else {
                $usucpf = array("'{$usucpf}'");
            }
            foreach ($usucpf as $k => $v) {
                if (trim($v) == '' || $v == "''" || empty($v)) {
                    unset($usucpf[$k]);
                }
            }
            if (!empty($usucpf)) {
                $where[] = " ckf.usucpf IN ( " . implode(",", $usucpf) . " ) ";
            }
        }
        
        if (!empty($estuf)) {
            if (is_array($estuf)) {
                foreach ($estuf as $k => $v) {
                    $estuf[$k] = "'{$v}'";
                }
            } else {
                $estuf = array("'{$estuf}'");
            }
            foreach ($estuf as $k => $v) {
                if (trim($v) == '' || $v == "''" || empty($v)) {
                    unset($estuf[$k]);
                }
            }
            if (!empty($estuf)) {
                $where[] = " mun.estuf IN ( " . implode(",", $estuf) . " ) ";
            }
        }
        
        if (!empty($muncod)) {
            if (is_array($muncod)) {
                foreach ($muncod as $k => $v) {
                    $muncod[$k] = "'{$v}'";
                }
            } else {
                $muncod = array("'{$muncod}'");
            }
            foreach ($muncod as $k => $v) {
                if (trim($v) == '' || $v == "''" || empty($v)) {
                    unset($muncod[$k]);
                }
            }
            if (!empty($muncod)) {
                $where[] = " mun.muncod IN ( " . implode(",", $muncod) . " ) ";
            }
        }

        if (!empty($esdid_obr) && is_array($esdid_obr) && $esdid_obr[0] != '') {
            $where[] = " esd_obr.esdid IN ( " . implode(",", $esdid_obr) . " ) ";
        }
        
        if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
            $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
        }
        
        if ( !empty($tooid) && count($tooid) > 0 && $tooid[0] !== ''){
            $where[] = " too.tooid IN('" . implode("', '", $tooid) . "')";
        }
        
        if(!empty($ckfdatainclusao_de) && !empty($ckfdatainclusao_ate)){
            $de  = explode('/', $ckfdatainclusao_de);
            $ate = explode('/', $ckfdatainclusao_ate);
            $ckfdatainclusao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $ckfdatainclusao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " ckf.ckfdatainclusao BETWEEN '{$ckfdatainclusao_de}' AND '{$ckfdatainclusao_ate}' ";
        }elseif(!empty($ckfdatainclusao_de) && empty($ckfdatainclusao_ate)){
            $de  = explode('/', $ckfdatainclusao_de);
            $ckfdatainclusao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " ckf.ckfdatainclusao = '{$ckfdatainclusao_de}' ";
        }elseif(empty($ckfdatainclusao_de) && !empty($ckfdatainclusao_ate)){
            $ate = explode('/', $ckfdatainclusao_ate);
            $ckfdatainclusao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " ckf.ckfdatainclusao = '{$ckfdatainclusao_ate}' ";
        }
        
        if(!empty($htddata_de) && !empty($htddata_ate)){
            $de  = explode('/', $htddata_de);
            $ate = explode('/', $htddata_ate);
            $htddata_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $htddata_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " hst_ck.htddata BETWEEN '{$htddata_de}' AND '{$htddata_ate}' ";
        }elseif(!empty($htddata_de) && empty($htddata_ate)){
            $de  = explode('/', $htddata_de);
            $htddata_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
            $where[] = " hst_ck.htddata = '{$htddata_de}' ";
        }elseif(empty($htddata_de) && !empty($htddata_ate)){
            $ate = explode('/', $htddata_ate);
            $htddata_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
            $where[] = " hst_ck.htddata = '{$htddata_ate}' ";
        }
        
        return $where;
    }

    public function getListaChecklistGeral($tipo_retorno = 'sql', $where_interno = '') {
        $dados = array();
        
        $where     = $this->getFiltrosDadosListaChecklistGeral();
        $strWhere  = ( (!empty($where)) ? ' AND ' . implode(' AND ', $where) : '');
        $strWhere .= $where_interno;
        
      /*  $sql = "  
                     SELECT 
                        
                       '<a href=\"obras2.php?modulo=principal/cadValidacao&acao=A&obrid=' || obr.obrid || '\">' || obr.obrid || '</a>'  as \"ID da Obra\", 
                       obr.obrnome as \"Nome da Obra\", 
                       mun.estuf as \"UF\", 
                       mun.mundescricao as \"Município\", 
                       esd_obr.esddsc as \"Situação da Obra\", 
                       obr.obrpercentultvistoria as \"% Executado\", 
                       --\"Ação\", 
                       ckf.ckfid, 
                       CASE
                            WHEN que.queid = ".QUEID_QUEST_CHKLST_ADM." 
                            THEN 'Administrativo'
                            WHEN que.queid = ".QUEID_QUEST_CHKLST_TEC." 
                            THEN 'Técnico'
                            WHEN que.queid = ".QUEID_QUEST_CHKLST_2P." 
                            THEN '2° Parcela'
                            WHEN que.queid = ".QUEID_QUEST_CHKLST_OBR_VINC." 
                            THEN 'Obra Vinculada'
                       ELSE
                           'Checklist - Modelo Antigo'
                       END as \"Tipo\", 
                       usu.usunome as \"Responsável\",
                       TO_CHAR(ckf.ckfdatainclusao, 'DD/MM/YYYY') as \"Data de Inclusão\", 
                       esd_ck.esddsc as \"Situação\", 
                       COALESCE(to_char(hst_ck.htddata, 'DD/MM/YYYY')) AS \"Data da Situação\" 
                       --,\"Possui Pendências\"

                     FROM obras2.checklistfnde          ckf
                     
                     INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                     INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                     LEFT JOIN obras2.obras                       obr ON obr.obrid  = ckf.obrid
                     INNER JOIN obras2.empreendimento              ep ON ep.empid   = obr.empid
                     LEFT JOIN workflow.documento           doc_ck ON doc_ck.docid  = ckf.docid
                     LEFT JOIN workflow.estadodocumento     esd_ck ON esd_ck.esdid  = doc_ck.esdid
                     LEFT JOIN workflow.historicodocumento  hst_ck ON hst_ck.hstid  = doc_ck.hstid
                     LEFT JOIN workflow.documento          doc_obr ON doc_obr.docid = obr.docid
                     LEFT JOIN workflow.estadodocumento    esd_obr ON esd_obr.esdid = doc_obr.esdid
                     LEFT JOIN obras2.programafonte                pf ON pf.prfid   = ep.prfid
                     LEFT JOIN obras2.tipoorigemobra              too ON too.tooid  = obr.tooid
                     LEFT JOIN obras2.tipologiaobra               tpo ON tpo.tpoid  = obr.tpoid
                     LEFT JOIN entidade.endereco                 ende ON ende.endid = obr.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
                     LEFT JOIN territorios.municipio              mun ON mun.muncod = ende.muncod
                     LEFT JOIN territorios.estado                  uf ON mun.estuf  = uf.estuf
                     LEFT JOIN seguranca.usuario                  usu ON usu.usucpf = ckf.usucpf 
                     
                     WHERE ckf.ckfstatus = 'A'
                     " . $strWhere . " 
                     
                     ORDER BY obr.obrid

                     "; */
                        
        if($tipo_retorno == 'sql'){
            return $sql;
        }else{
            $dados = $this->carregar($sql);
            $dados = (!empty($dados)) ? $dados : array();
            $dados = $this->montaColunaPendenciasChkLst($dados);
            
            if(!empty($_POST['possui_pendencia']) && !empty($dados)){
                switch ($_POST['possui_pendencia']) {
                    case 'S':
                        foreach ($dados as $key => $value) {
                            if($value['pendencias'] == 'Não'){
                                unset($dados[$key]);
                            }
                        }
                        break;
                    case 'N':
                        foreach ($dados as $key => $value) {
                            if($value['pendencias'] == 'Sim'){
                                unset($dados[$key]);
                            }
                        }
                        break;
                }
                $dados_finais = array();
                
                // "Reseta" os índices do array, para não dar pau no $db->monta_lista
                $c = 0;
                foreach ($dados as $value) { $dados_finais[$c] = $value; $c++; }
                
            }else{
                $dados_finais = $dados;
            }
            return $dados_finais;
        }
    }
    
    public function getListaCheck($tipo, $where = '') {
        
        switch ($tipo) {
            case 'lista':
                $campos = ' clc.clcid, 
                            clc.qrpid,
                            que.queid, 
                            usu.usunome, 
                            clc.clcdatainclusao, 
                            esd.esddsc, 
                            COALESCE(to_char(hst.htddata, \'DD/MM/YYYY\')) AS htddata,
                            COALESCE(to_char(clc.clcdatainclusao, \'DD/MM/YYYY\')) AS clcdatainclusao';
                break;
            case 'completo':
                $campos = ' 
                            clc.*,
                           
                          
                            qrp.*,
                            que.*,
                            usu.*
                           ';
                break;
            default:
                $campos = ' clc.clcid, que.queid, usu.usunome, clc.clcdatainclusao, esd.esddsc, hst.htddata ';
                break;
        }
        
       
        
        $obrid = (empty($_SESSION['obras2']['checklistfnde']['obrid'])) ? $_SESSION['obras2']['obrid'] : $_SESSION['obras2']['checklistfnde']['obrid'];

        $sql = " SELECT {$campos} 
             FROM obras2.checklistcumprimento                   clc
       
             INNER JOIN questionario.questionarioresposta qrp ON clc.qrpid  = qrp.qrpid
             INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
             INNER JOIN seguranca.usuario                 usu ON clc.usucpf = usu.usucpf
             WHERE clc.clcstatus = 'A' 
             AND clc.obrid       = " . $obrid . " 
             {$where}
             ORDER BY clc.clcdatainclusao
          ";
             
              //ver($sql,d);

        $lista = $this->carregar($sql);

        $lista = (empty($lista) || $lista == false) ? array() : $lista;

        return $lista;
    }

    /**
     * 
     * @param type $tipo  adm/tec/2p
     * @param type $obrid 1
     * @param type $ckfid 1
     * @param type $acao  duplicar/editar/cadastrar/excluir;
     */
    public function mostraPopupChecklistQuestionario($tipo = 'adm', $obrid = 0, $ckfid = 0, $acao = 'cadastrar') {

        if (isset($_SESSION['obras2']['checklistfnde']) && $_SESSION['obras2']['checklistfnde']['onscreen'] == true) {
            $tipo  = $_SESSION['obras2']['checklistfnde']['tipo'];
            $obrid = $_SESSION['obras2']['checklistfnde']['obrid'];
            $ckfid = $_SESSION['obras2']['checklistfnde']['ckfid'];
            $acao  = $_SESSION['obras2']['checklistfnde']['acao'];
        }

        require_once APPRAIZ . 'includes/workflow.php';
        include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
        include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        switch ($acao) {
            
            
           
            case 'editar':
                
               
                $queid = $this->getTipoChecklistFnde($ckfid);
               $qrpid = $this->pegaQrpidChk($ckfid, $queid);
                
//                ver($ckfid, $queid);
                break;
            
                die('
                  <script>
                    alert(\'' . $msg . '\');
                    window.close();
                  </script>
                ');
                break;
            
        }

        $where       = ' AND clc.clcid = ' . $ckfid;
        $dados       = $this->getListaCheck('completo', $where);
        $responsavel = $dados[0]['usunome'];
        $docid       = $dados[0]['docid_ckf'];
        $esdid       = $dados[0]['esdid'];
         $qrpid       = empty($dados[0]['qrpid']) ? $qrpid : $dados[0]['qrpid'];

        if (empty($qrpid) || $qrpid == 'NULL' || $qrpid == 0) {
            $qrpid = $_SESSION['obras2']['checklistfnde']['qrpid'];
        }
        
        if (empty($qrpid) || $qrpid == 'NULL' || $qrpid == 0) {
            $queid = 0;
        }else{
            $queid = $this->pegaUm('SELECT queid FROM questionario.questionarioresposta WHERE qrpid = ' . $qrpid);
        }

      
      //  $perid = 3744;
        
       // echo $obrid;
        
        
     
        $this->printHtmlPopupChecklistQuestionario("Questionário", "Cumprimentos de objetos", $obrid, $ckfid, $docid, $qrpid, $perid, $queid, $responsavel, $habilitado);

        // Variável setada para quando for precionado o botão de "Próximo" do questionário, não recarregue a tela inteira no fieldset, mas só o questionário.
        $_SESSION['obras2']['checklistfnde']['onscreen'] = true;
        $_SESSION['obras2']['checklistfnde']['acao']     = $acao;
        $_SESSION['obras2']['checklistfnde']['tipo']     = $tipo;
        $_SESSION['obras2']['checklistfnde']['obrid']    = $obrid;
        $_SESSION['obras2']['checklistfnde']['ckfid']    = $ckfid;
        $_SESSION['obras2']['checklistfnde']['qrpid']    = $qrpid;

        die();
    }

    public function printHtmlPopupChecklistQuestionario($nome_titulo, $sub_titulo, $obrid, $ckfid, $docid, $qrpid, $perid, $queid, $responsavel, $habilitado) {

        require_once APPRAIZ . 'includes/workflow.php';
        include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
        include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        echo '  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
                <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
                <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
                <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
                <script language="JavaScript" src="../includes/funcoes.js"></script>
                <script type="text/javascript">

                      function printQuestionario(ckfid){
                          var url = "/obras2/obras2.php?modulo=principal/questionarioImpressaoChecklistValid&acao=A&queid=' . $queid . '";
                          popup2 = window.open(
                              url,
                              "_blank",
                              "width=1000,height=650,scrollbars=yes,scrolling=no,resizebled=no"
                          );
                      }

                      function closeWindow(){
                          // Antes de fechar a janela, faz uma chamada ajax para zerar as variáveis de sessão do checklist
                          var Data = $.ajax({
                              type : "GET",
                              url : "/obras2/obras2.php?modulo=principal/cadValidacao&acao=A&requisicao=resetchecklist",
                              cache : false,
                              global : false,
                              async : false,
                              success : function(data) {
                                  return data;
                              }
                          }).responseText;
                          opener.location.reload();
                          window.close();
                      }

                      // Ao clicar no botão de fechar a janela, chama a função "closeWindow"
                      window.onbeforeunload = closeWindow;

                </script>
                ';

        // Verificação da Variável de controle de tela. 
        // Quando for a "primeira vez" em que a tela é carregada, todo o conteúdo é mostrado. 
        // Depois da primeira vez (popup aberta), somente o questionário é carregado.
        if (!isset($_SESSION['obras2']['checklistfnde']['onscreen'])) {
            monta_titulo("Checklist " . $nome_titulo, $sub_titulo);
            echo cabecalhoObra($obrid);
            ?>
            <table bgcolor="#f5f5f5" align="center" class="tabela" >
                <tr>
                    <td colspan="3" class="subtitulocentro">Identificação da Análise</td>
                </tr>
                <tr>
                    <td class="subtitulodireita" style="width: 15%">ID da Análise:</td>
                    <td style="width: 75%"><?php echo $ckfid; ?></td>
                    <td rowspan="3" style="width: 10%; vertical-align: middle; text-align: center">
                      
                        <br />
                        <table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
                            <tbody>
                                <tr style="background-color: rgb(201, 201, 201); text-align: center;">
                                    <td style="font-size: 7pt; text-align: center;">
                                        <span title="estado atual">
                                            <b>Análise</b>
                                        </span>
                                    </td>
                                </tr>
                                <tr style="text-align: center;">
                                    <td style="font-size: 7pt; text-align: center;"> <a href="#" id="print_quest" onclick="printQuestionario(<?php echo $ckfid; ?>)"> Imprimir </a> </td>
                                </tr>
                        </table>

                    </td>
                </tr>
                <tr>
                    <td class="subtitulodireita">Responsável:</td>
                    <td colspan="2"><?php echo $responsavel; ?></td>
                </tr>
                <tr>
                    <td colspan="2" style="width: 900px; max-width: 920px; overflow: auto;">

                        <table bgcolor="#f5f5f5" align="center" class="tabela" style="width: 900px; max-width: 920px;">
                            <tr>
                                <td>
                                    <fieldset style="width: 94%; background: #fff;" >
                                        <legend>Questionário</legend>
                                        
                                        
                                        
                                        <?php
                                       
                                            $tela = new Tela(array('qrpid'        => $qrpid,
                                                                   'tamDivArvore' => 25,
                                                                   'tamDivPx'     => 250,
                                                                   'habilitado'   => $habilitado));
                                        ?>
                                    </fieldset>
                                </td>
                            </tr>
                        </table>

                    </td>
                </tr>
            </table>    
            <?php
        } else {
            ?>  
            <table bgcolor="#f5f5f5" align="center" class="tabela" >
                <tr>
                    <td>
                        <fieldset style="width: 94%; background: #fff;" >
                            <legend>Questionário</legend>
                            <?php
                                $tela = new Tela(array('qrpid' => $qrpid,
                                                       'tamDivArvore' => 25,
                                                       'tamDivPx' => 250,
                                                       'habilitado' => $habilitado));
                            ?>
                        </fieldset>
                    </td>
                </tr>
            </table>
            <?php
        }
    }

    public function montaColunaPendenciasChkLst($lista) {
        foreach ($lista as $key => $value) {            
            $ckfid = (empty($value['ckfid'])) ? 0 : $value['ckfid'];
            $tipo  = $this->getTipoChecklistFnde($ckfid);
            
            switch ($tipo) {
                case QUEID_QUEST_CHKLST_TEC:
                    $tec = $this->montaArrayDadosPendenciaChecklistTec($ckfid);
                    $lista[$key]['pendencias'] = (count($tec) > 0) ? 'Sim' : 'Não';
                    break;
                case QUEID_QUEST_CHKLST_2P:
                    $sp  = $this->montaArrayDadosPendenciaChecklist2P($ckfid);
                    $lista[$key]['pendencias'] = (count($sp) > 0) ? 'Sim' : 'Não';
                    break;
                case QUEID_QUEST_CHKLST_ADM:
                    $adm = $this->montaArrayDadosPendenciaChecklistAdm($ckfid);
                    $lista[$key]['pendencias'] = (count($adm) > 0) ? 'Sim' : 'Não';
                    break;
                case QUEID_QUEST_CHKLST_OBR_VINC:
                    $adm = $this->montaArrayDadosPendenciaChecklistObraVinculada($ckfid);
                    $lista[$key]['pendencias'] = (count($adm) > 0) ? 'Sim' : 'Não';
                    break;
                default:
                    break;
            }
            
            if($lista[$key]['pendencias'] == ''){
                $lista[$key]['pendencias'] = 'Não';
            }
            
        }
        return $lista;
    }

    public function criaDocidChecklist($ckfid) {
        if (empty($ckfid)) {
            return false;
        }
        require_once APPRAIZ . 'includes/workflow.php';
        $docdsc = "Fluxo do Checklist FNDE - ckfid: " . $ckfid;
        $docid = wf_cadastrarDocumento(TPID_CHECKLIST_VALIDACAO, $docdsc);
        return $docid;
    }

    public function pegaQrpidChk($ckfid, $queid) {
        include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
        $sql = "SELECT
            	clc.qrpid as qrpid
            FROM
            	obras2.checklistcumprimento clc
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = clc.qrpid
            WHERE clc.clcid = {$ckfid}
              AND q.queid   = {$queid}";
       
        $dados = $this->pegaLinha($sql);
        
      
        if (empty($dados['qrpid'])) {
            $arParam = array("queid" => $queid, "titulo" => "Checklist da Validação (" . $ckfid . " - Checklist da Validação)");
            $qrpid = GerenciaQuestionario::insereQuestionario($arParam);
            $sql = "UPDATE
                        obras2.checklistcumprimento
                    SET
                        qrpid = {$qrpid}
                    WHERE
                        clcid = {$ckfid}";
            $this->executar($sql);
            $this->commit();
        } else {
            $qrpid = $dados['qrpid'];
        }

        return $qrpid;
    }

    public function getTipoChecklistFnde($ckfid) {
        $sql = "SELECT
            	q.queid as queid
            FROM
            	obras2.checklistcumprimento clc
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = clc.qrpid
            WHERE
            	clc.clcid   = {$ckfid}";
        $queid = $this->pegaUm($sql);
        return $queid;
    }

    public function montaArrayVerificacaoPendenciasChkLst($ckfid = null, $qrpid = null, $obrid = null) {
        $campos = ' * ';
        $where  = '';
        if(!empty($ckfid)){
            $where .= " AND ckf.ckfid = " . $ckfid ;
        }
        if(!empty($qrpid)){
            $where .= " AND qrp.qrpid = " . $qrpid ;
        }
        if(!empty($obrid)){
            $where .= " AND ckf.obrid = " . $obrid ;
        }
        $sql = "SELECT ".$campos."
                FROM obras2.checklistfnde ckf
                INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid = qrp.qrpid
                INNER JOIN questionario.resposta             qre ON qrp.qrpid = qre.qrpid
                INNER JOIN questionario.itempergunta         itp ON qre.itpid = itp.itpid
                WHERE ckf.ckfstatus = 'A' ".$where;
        
        $resp = $this->carregar($sql);
        $resp = (empty($resp)) ? array() : $resp;
        return $resp;        
    }
    
    public function montaArrayDadosPendenciaChecklistObraVinculada($ckfid, $obrid = null) {
        $arrDados = array();
        $resp     = $this->montaArrayVerificacaoPendenciasChkLst($ckfid, null, $obrid);
        $c        = 0;
        foreach ($resp as $k => $v) {
            //Se ouver alguma resposta 'Não', então existe pendências
            if (trim(str_to_upper($v['itptitulo'])) == 'NÃO') {
                switch ($v['perid']) {
                    case 3847: // 1.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A homologação da primeira licitação não foi inserida no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir documento de homologação da primeira licitação realizada, na aba documentos.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3848: // 2.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'O contrato com a primeira empresa executora da obra não foi inserido no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir contrato original com a primeira empresa executora da obra, na aba documentos.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3849: // 2.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A ordem de serviço emitida pelo ente, para início da obra pela primeira empresa contratada não foi inserida no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir ordem de serviço emitido pelo ente para início da obra, na aba Documentos.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3850: // 2.3
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A planilha de custos referente a primeira licitação não foi inserida no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir planilha de custos referente à primeira licitação na aba documentos.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3851: // 3.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A rescisão contratual não encontra-se inserida no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir rescisão contratual com a empresa executora da obra.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia a criação da obra vinculada.';
                        $arrDados[$c]['tipo_pendencia'] = 'B';
                        break;
                    case 3852: // 3.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'O boletim de medição acumulada com o total executado pela empresa que teve o contrato rescindido não encontra-se inserido no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir boletim de medição acumuluda demonstrando o total de obra executado pela empresa que teve o contrato, na aba documentos';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia a criação da obra vinculada.';
                        $arrDados[$c]['tipo_pendencia'] = 'B';
                        break;
                    case 3853: // 3.3
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A ART de execução referente à empresa que teve o contrato rescindido não encontra-se inserido no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir ART de execução em nome da empresa que teve o contrato rescindido.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia a criação da obra vinculada.';
                        $arrDados[$c]['tipo_pendencia'] = 'B';
                        break;
                    case 3854: // 3.4
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A ART de fiscalização em nome do fiscal (engenheiro/arquiteto) responsável pela obra não encontra-se inserido no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir ART de fiscalização em nome do fiscal responsável pela obra.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia a criação da obra vinculada.';
                        $arrDados[$c]['tipo_pendencia'] = 'B';
                        break;
                    case 3855: // 4.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A publicação da abertura de licitação para nova contratação não encontra-se inserido no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir publicação da abertura de licitação para contratação de nova  empresa para conclusão da obra, na aba documentos.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia a criação da obra vinculada.';
                        $arrDados[$c]['tipo_pendencia'] = 'B';
                        break;
                    case 3856: // 5.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'As notas fiscais emitidas pela empresa que teve o contrato rescindido não foram inseridas no sistema ou encontra-se incompleto.';
                        $arrDados[$c]['providencia']    = 'Inserir notas fiscais emitidas pela empresa que teve o contrato rescindido na aba documentos.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3857: // 5.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Os comprovantes de pagamentos feitos à empresa que teve o contrato rescindido não encontram-se no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir comprovantes de pagamento feitos à empresa que teve o contrato rescindido na aba documentos.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                }
                $c++;
            }
        }
        
        return $arrDados;
    }
    
    public function montaArrayDadosPendenciaChecklistTec($ckfid, $obrid = null) {
        $arrDados = array();
        $resp     = $this->montaArrayVerificacaoPendenciasChkLst($ckfid, null, $obrid);
//        ver($resp);
        $c        = 0;
        foreach ($resp as $k => $v) {
            //Se ouver alguma resposta 'Não', então existe pendências
            if (trim(str_to_upper($v['itptitulo'])) == 'NÃO') {
                switch ($v['perid']) {
                    case 3834: // 3.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'O cronograma informado na aba cronograma físico-financeiro está com as datas defasadas.';
                        $arrDados[$c]['providencia']    = 'Atualizar cronograma com as datas de realização dos serviços';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3846: // 3.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A obra está atrasada em relação ao cronograma informado na aba cronofgrama físico-financeiro.';
                        $arrDados[$c]['providencia']    = ' Atualizar as datas de execução das etapas do cronograma.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3835: // 3.3
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Na planilha cadastrad na aba cronograma físico-financeiro a não consta dodos os macro serviços acordados com o FNDE.';
                        $arrDados[$c]['providencia']    = 'Verificar se os itens macros das etapas, conforme planilha acordada, estão inseridos e com os valores compatíveis com os pactuados com o FNDE.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3836: // 3.4
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Os serviços não pactuados com o FNDE devem estar separados no item serviços não pactuados com o FNDE na aba cronograma físico financeiro.';
                        $arrDados[$c]['providencia']    = 'Apresentar o item que não foi objeto da pactuação com o FNDE, com seus quantitativos e valores, separadamente no cronograma, mancando a opção "Serviços não pactuados com o FNDE".';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3838: //4.1.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Os percentuais informados na vistoria não estão coerentes com as informações do relatório técnico e imagens inseridas.';
                        $arrDados[$c]['providencia']    = 'Rever percentual de serviços executados. Melhorar o relatório técnico descrevendo sobre todos os serviços realizados tanto qualitativamente como quantitativamente, verificando a conformidade ou não-conformidade com o projeto acordado com o FNDE.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3839: //4.1.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'O relatório técnico de cadastrado na aba vistoria não apresenta informações sobre a execução dos serviços, sua qualidade e conformidade com o objeto acordado.';
                        $arrDados[$c]['providencia']    = 'O Relatório técnico enquanto texto deve apresentar os serviços executados relatando sua qualidade e conformidade com o especificado no projeto pactuado, bem como relatar as divergências e motivos pelo qual estes foram adotados sem a prévia autorização do FNDE.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3840:  //4.1.3
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A quantidade e qualidade das imagens cadastradas na aba vistorias não permitem a avaliação da execução física da obra.';
                        $arrDados[$c]['providencia']    = 'Inserir fotografias legendadas de todos os serviços executados e visão geral da obra. Ver posicionamento mínimo apresentado no Manual de Monitoramento Obras 2.0.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3841:  //4.1.4
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A Obra está sendo executada em desconformidade com o projeto pactuado com o FNDE.';
                        $arrDados[$c]['providencia']    = 'Encaminhar em meio físico e inserir na aba DOCUMENTOS, ofício assinado pelo gestor público justificando a alteraçãodo projeto, incluindo em anexo cópia do projeto executivo (com levantamento do como foi construído em relação ao projeto-USBUILT), ART desse projeto, planilha orçamentária, memorial descritivo, especificações de material, e outros detalhes julgados necessários para compreensão da proposta.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3842:  //4.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'As informações inseridas na aba vistoria e na aba recursos estão com informações incompatíveis em relação aos valores repassados e executados.';
                        $arrDados[$c]['providencia']    = 'Inserir nova vistoria que comprove o avanço físico da obra, conforme pactuado no Termo de Compromisso/Convênio.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3844:  //5.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A Aba execução orçamentária está sem preenchimento ou com informações faltando sobre os pagamentos realizados à empresa executora.';
                        $arrDados[$c]['providencia']    = 'Inserir todos os boletins de medição e notas fiscais, documentos assinados e digitalizados na extensão PDF.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
//                    case 3845:  //6.1
//                        $arrDados[$c]['obrid']          = $v['obrid'];
//                        $arrDados[$c]['preid']          = $v['preid'];
//                        $arrDados[$c]['ckfid']          = $v['ckfid'];
//                        $arrDados[$c]['descricao']      = 'A obra possui restrições impedidtivas de pagamento.';
//                        $arrDados[$c]['providencia']    = 'Ver aba restrições/inconformidades, identificar as restrições ainda não superadas, e atender ao indicado.';
//                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
//                        $arrDados[$c]['tipo_pendencia'] = 'R';
//                        break;
                }
                $c++;
            }
        }
        
//        ver($arrDados);
        
        return $arrDados;
    }
    
    public function montaArrayDadosPendenciaChecklist2P($ckfid = null, $obrid = null) {
        $arrDados = array();
        
        $resp = $this->montaArrayVerificacaoPendenciasChkLst($ckfid, null, $obrid);
        $c = 0;
        foreach ($resp as $k => $v) {
            //Se ouver alguma resposta 'Não', então existe pendências
            if (trim(str_to_upper($v['itptitulo'])) == 'NÃO') {
                switch ($v['perid']) {
                    case 3744://1.1                                
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Aba "Contratação" não preenchida.';
                        $arrDados[$c]['providencia']    = 'Preencher aba contratação, inserindo: Contrato com a empresa, Ordem de Serviço e Planilha contratada; todos esses documentos devidamente datados e assinados.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3745://1.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'O contrato encontra-se vencido.';
                        $arrDados[$c]['providencia']    = 'Inserir termo aditivo de prazo ao contrato. Utilizar link "Inserir aditivo", localizado na barra de menus.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3746://1.3
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Ordem de serviço não inserida na aba "Contratação", ou documento anexado não se trata da Ordem de serviço.';
                        $arrDados[$c]['providencia']    = 'Inserir Ordem de serviço na aba contratação. Se a obra estiver em execução, utilize o link "Editar Contrato", localizado na barra de menus, para a inserção deste documento.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3747://1.4
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A Ordem de serviço inserida não está assinada.';
                        $arrDados[$c]['providencia']    = 'Inserir Ordem de Serviço,  datada e assinada, na aba contratação. Se a obra estiver em execução, utilize o link "Editar Contrato" para a inserção deste documento.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3748://1.5
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = ' As informações digitadas e/ou inseridas no campo Ordem de Serviço estão em desacordo com a homologação da licitação.';
                        $arrDados[$c]['providencia']    = 'Providenciar a correção e/ou inserção das informações inseridas no campo ordem de serviço da aba contratação, deixando de acordo com a homologação da licitação.  Se a obra estiver em execução, utilize o link "Editar Contrato", localizado na barra de menus, para a correção/inserção deste documento.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3749://1.6
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'A planilha orçamentária não está inserida no sistema.';
                        $arrDados[$c]['providencia']    = 'Inserir planilha orçamentária, no valor contratado pelo município, devidamente assinada. Se a obra estiver em execução, utilize o link "Editar Contrato", localizado na barra de menus, para a correção/inserção deste documento.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    default:
                        break;
                }
                $c++;
            }else{ // Sim
                if($v['perid'] == 3749){
                    foreach ($resp as $k2 => $v2) {
                        if($v2['perid'] == 3750 && trim(str_to_upper($v2['itptitulo'])) == 'NÃO'){
                            $arrDados[$c]['obrid']          = $v['obrid'];
                            $arrDados[$c]['preid']          = $v['preid'];
                            $arrDados[$c]['ckfid']          = $v['ckfid'];
                            $arrDados[$c]['descricao']      = 'A planilha orçamentária inserida está com valor divergente do digitado na aba contratação.';
                            $arrDados[$c]['providencia']    = 'Inserir planilha orçamentária com valor igual ao digitado na aba contratação, ou corrigir o valor digitado, deixando conforme o documento anexado.';
                            $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                            $arrDados[$c]['tipo_pendencia'] = 'R';
                        }
                    }
                }
            }
        }
        
        return $arrDados;
    }
    
    public function montaArrayDadosPendenciaChecklistAdm($ckfid = null, $obrid = null, $acao_validacao_retorno = null) {
        $arrDados = array();
        $resp = $this->montaArrayVerificacaoPendenciasChkLst($ckfid, null, $obrid);
        $c = 0;
        foreach ($resp as $k => $v) {
            //Se ouver alguma resposta 'Não', então existe pendências
            if (trim(str_to_upper($v['itptitulo'])) == 'NÃO') {
                switch ($v['perid']) {
                    //1
                    case 3765://1.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'O contrato encontra-se vencido.';
                        $arrDados[$c]['providencia']    = 'Inserir termo aditivo de prazo ao contrato. Utilizar link "inserir aditivo", localizado na barra de menus.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3767://1.1.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Falta inserir aditivo de prazo para que o contrato permaneça vigente.';
                        $arrDados[$c]['providencia']    = 'Inserir termo aditivo de prazo ao contrato. Utilizar link "inserir aditivo", localizado na barra de menus.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3768://1.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Falta inserir Ordem de Serviço';
                        $arrDados[$c]['providencia']    = 'Inserir Ordem de Serviço, para início da obra com a empresa contratada, datada e assinada, na aba contratação.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3769://1.2.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = ' As informações digitadas na aba contratação estão em desacordo com os dados da Ordem de serviço anexada.';
                        $arrDados[$c]['providencia']    = 'Providenciar a correção das informações declaradas no campo ordem de serviço da aba contratação, deixando de acordo com o arquivo anexo deste documento. Utilizar o link "Editar Contrato", localizado na barra de menus.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3770://1.3
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Falta inserir planilha contratada (assinada) na aba contratação, com valor total igual ao contratado.';
                        $arrDados[$c]['providencia']    = 'Inserir planilha contratada (assinada) na aba contratação com valor total igual ao contratado.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3771://1.3.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'O valor total da planilha está em desacordo com o valor digitado na aba contratação.';
                        $arrDados[$c]['providencia']    = 'Inserir planilha contratada (assinada) na aba contratação com valor total igual ao contratado, ou corrigir as informações declaradas no campo da planilha contratada, deixando tais informações condizentes com o arquivo anexo. Utilizar o link "Editar Contrato", localizado na barra de menus.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    //2
                    case 3772://2.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Falta ART/RRT de execução.';
                        $arrDados[$c]['providencia']    = 'Inserir ART de execução registrada e paga (em nome da empresa declarada na aba contratação) na aba Vistoria.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3773://2.1.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Os dados constantes na ART/RRT de execução estão em desacordo com as informações da empresa cadastrada na aba contratação';
                        $arrDados[$c]['providencia']    = 'Inserir ART de execução registrada e paga (em nome da empresa cadastrada na aba contratação) na aba Vistoria.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
//                            case ://2.1.2 // Não existe no checklist, ficar atento para qnd for criada a pergunta, colocar o ID aqui e descomentar
//                                $arrDados[$c]['descricao']   = 'Os dados constantes na ART/RRT de execução estão em desacordo com as informações da empresa cadastrada na aba contratação';
//                                $arrDados[$c]['providencia'] = 'Inserir ART de execução registrada e paga (em nome da empresa cadastrada na aba contratação) na aba Vistoria.';
//                                break;
                    case 3774://2.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Falta ART/RRT de fiscalização.';
                        $arrDados[$c]['providencia']    = 'Inserir ART de fiscalização registrada e paga (em nome de profissional vinculado à obra, com dados corretos) na aba vistoria.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
                    case 3775://2.2.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Os dados constantes na ART/RRT de fiscalização estão em desacordo com os dados do fiscal (engenheiro/arquiteto) vinculado à obra.';
                        $arrDados[$c]['providencia']    = 'Inserir ART de fiscalização registrada e paga (em nome de profissional vinculado à obra, com dados corretos) na aba vistoria.';
                        $arrDados[$c]['acao_validacao'] = 'Bloqueia recurso';
                        $arrDados[$c]['tipo_pendencia'] = 'R';
                        break;
//                            case ://2.2.2 // Não existe no checklist, ficar atento para qnd for criada a pergunta, colocar o ID aqui e descomentar
//                                $arrDados[$c]['descricao']   = '';
//                                $arrDados[$c]['providencia'] = '';
//                                break;
                    //3
                    case 3776://3.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Falta dados de pagamentos e medições na aba execução orçamentária.';
                        $arrDados[$c]['providencia']    = 'Inserir comprovantes de pagamentos, notas fiscais e boletins de medição na aba execução orçamentária.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3777://3.1.1
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Os dados das notas fiscais estão em desacordo com as informações da empresa contratada declarados na aba contratação. ';
                        $arrDados[$c]['providencia']    = '1 - Inserir notas fiscais na aba execução orçamentária com dados condizentes com o declarado na aba contratação, ou <br> 2- Corrigir os dados da aba contratação, caso o erro se encontre nas informações inseridas nesta aba, utilizando o link "Editar Contrato, localizado na barra de menus.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3778://3.1.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Os lançamentos de pagamento estão desatualizados.';
                        $arrDados[$c]['providencia']    = 'Inserir notas fiscais e comprovantes de pagamentos atualizados com prazo inferior a 60 dias para obras municípais e 90 dias para obras estaduais.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;
                    case 3779://3.2
                        $arrDados[$c]['obrid']          = $v['obrid'];
                        $arrDados[$c]['preid']          = $v['preid'];
                        $arrDados[$c]['ckfid']          = $v['ckfid'];
                        $arrDados[$c]['descricao']      = 'Falta inserir Boletins de Medição.';
                        $arrDados[$c]['providencia']    = 'Inserir boletins de medição na aba execução orçamentária dos valores comprovadamente pagos declarados em Nota Fiscal.';
                        $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                        $arrDados[$c]['tipo_pendencia'] = 'I';
                        break;                    
                    default:
                        break;
                }
            }else{
                if ($v['perid'] == 3766){ //1.1.1
                    $arrDados[$c]['obrid']          = $v['obrid'];
                    $arrDados[$c]['preid']          = $v['preid'];
                    $arrDados[$c]['ckfid']          = $v['ckfid'];
                    $arrDados[$c]['descricao']      = 'O contrato vencerá nos próximos 30 dias.';
                    $arrDados[$c]['providencia']    = 'Inserir o termo aditivo de  prazo, evitando que o contrato perca a vigência. Utilizar link "inserir aditivo", localizado na barra de menus.';
                    $arrDados[$c]['acao_validacao'] = 'Pendência de preenchimento';
                    $arrDados[$c]['tipo_pendencia'] = 'I';                            
                }
            }
            $c++;
        }
        
        if(!empty($acao_validacao_retorno)){
            switch ($acao_validacao_retorno) {
                case 'pp': //Pendência de preenchimento
                    foreach ($arrDados as $key => $value) {
//                        if($value['acao_validacao'] != 'Pendência de preenchimento'){
                        if($value['tipo_pendencia'] != 'I'){
                            unset($arrDados[$key]);
                        }
                    }
                    break;
                case 'br': //Bloqueia recurso
                    foreach ($arrDados as $key => $value) {
//                        if($value['acao_validacao'] != 'Bloqueia recurso'){
                        if($value['tipo_pendencia'] != 'R'){
                            unset($arrDados[$key]);
                        }
                    }
                    break;
            }
        }
        return $arrDados;
    }
    
    /**
     *
     * @param type $obrid
     * @return boolean  true - para bloqueada; false - para não bloqueada
     */    
    public function verificaBloqueioDeVinculoViaChecklistObraVinculada($obrid){
        $pendencias = array();        
        $campos = ' ckf.ckfid ';
        $where  = " AND ckf.obrid = " . $obrid ;
        $where .= " AND qrp.queid = " . QUEID_QUEST_CHKLST_OBR_VINC ;        
        $sql = "SELECT ".$campos."
                FROM obras2.checklistfnde ckf
                INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid = qrp.qrpid
                INNER JOIN questionario.resposta             qre ON qrp.qrpid = qre.qrpid
                INNER JOIN questionario.itempergunta         itp ON qre.itpid = itp.itpid
                WHERE ckf.ckfstatus = 'A' ".$where;        
        $resp = $this->carregar($sql);
        $resp = (empty($resp)) ? array() : $resp;        
        foreach ($resp as $key => $value) {
            if(!empty($value['ckfid'])){
                $pendencias[] = $this->montaArrayDadosPendenciaChecklistObraVinculada($value['ckfid'], $obrid);
            }
        }        
        $flagBloqueioObraVinculada = false;
        foreach ($pendencias as $key => $value) {
            $arrDados[$c]['tipo_pendencia'] = 'B';
            if($value['tipo_pendencia'] == 'B'){
                $flagBloqueioObraVinculada = true;
                break;
            }
        }        
        return $flagBloqueioObraVinculada;
    }

    public function getEstadoChecklist($ckfid = null, $docid = null) {
        if (!empty($ckfid)) {
            $whereStr = ' AND ckf.ckfid = ' . $ckfid . ' ';
        }
        if (!empty($docid)) {
            $whereStr = ' AND ckf.docid = ' . $ckfid . ' ';
        }

        $sql = "SELECT 
                        
                        ckf.ckfid, ckf.obrid, o.empid,                        
                        ckf.usucpf as usu_cpf_cad_ckf,
                        ckf.docid,                         
                        usu_os.usunome as usu_nome_cad_ckf,
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
                            
                 FROM obras2.checklistfnde                 ckf
                 
                 INNER JOIN obras2.obras                 o ON ckf.obrid  = o.obrid
                 LEFT JOIN workflow.documento          doc ON doc.docid  = ckf.docid
                 LEFT JOIN workflow.tipodocumento      tpd ON tpd.tpdid  = doc.tpdid
                 LEFT JOIN workflow.estadodocumento    esd ON esd.esdid  = doc.esdid
                 LEFT JOIN workflow.historicodocumento hst ON hst.hstid  = doc.hstid
                 LEFT JOIN workflow.acaoestadodoc      aed ON aed.aedid  = hst.aedid                 
                 LEFT JOIN seguranca.usuario        usu_os ON ckf.usucpf = usu_os.usucpf 
                 LEFT JOIN seguranca.usuario      usu_acao ON hst.usucpf = usu_acao.usucpf 
                 
                 LEFT JOIN workflow.estadodocumento esdorigem  ON aed.esdidorigem  = esdorigem.esdid
                 LEFT JOIN workflow.estadodocumento esddestino ON aed.esdiddestino = esddestino.esdid

                 WHERE  ckf.ckfstatus = 'A' 
                        {$whereStr}
                 ";

        $res = $this->pegaLinha($sql);
        return $res;
    }

    public function getResponsaveisEmailChecklistFnde($ckfid) {
        $filtro_resp = ' ' . PFLCOD_SUPERVISOR_UNIDADE . ', ' . PFLCOD_GESTOR_UNIDADE;
        $arr         = $this->getEstadoChecklist($ckfid);
        if (!empty($arr)) {
            $empid = $arr['empid'];
        } else {
            return array();
        }

        $filtro_sql_resp = " AND ur.pflcod IN (" . $filtro_resp . ")";

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

    public function mandaEmailInconformidadeChecklist2P($ckfid) {
        $responsavel_destino = $this->getResponsaveisEmailChecklistFnde($ckfid);
        $this->montaEnviaEmailInconformidadeChecklist2P($ckfid, $responsavel_destino);
        $this->commit();
    }
    
    public function mandaEmailInconformidadeChecklistAdm($ckfid) {
        $responsavel_destino = $this->getResponsaveisEmailChecklistFnde($ckfid);
        $this->montaEnviaEmailInconformidadeChecklistAdm($ckfid, $responsavel_destino);
        $this->commit();
    }
    
    public function mandaEmailInconformidadeChecklistTec($ckfid) {
        $responsavel_destino = $this->getResponsaveisEmailChecklistFnde($ckfid);
        $this->montaEnviaEmailInconformidadeChecklistTec($ckfid, $responsavel_destino);
        $this->commit();
    }
    
    public function mandaEmailInconformidadeChecklistObraVinculada($ckfid) {
        $responsavel_destino = $this->getResponsaveisEmailChecklistFnde($ckfid);
        $this->montaEnviaEmailInconformidadeChecklistObraVinculada($ckfid, $responsavel_destino);
        $this->commit();
    }

    public function cadastraInconformidadeChecklistFnde($ckfid, $arrDadosPendencia = array(), $tipoChk = NULL) {        
        if(empty($this->obrid)){ $this->carregarPorId($ckfid); }        
        $arr_return = array();
        if (count($arrDadosPendencia) > 0) {
            foreach ($arrDadosPendencia as $key => $value) {
                
                if($value['tipo_pendencia'] != 'B'){
                    $obrid      = $this->obrid;
                    $objObra    = new Obras($obrid);
                    $empid      = $objObra->empid;
                    $estadoObra = $objObra->getEstadoObraWf($obrid);

                    $obrid      = empty($obrid) ? $_SESSION['obras2']['obrid'] : $obrid;
                    $empid      = empty($empid) ? $_SESSION['obras2']['empid'] : $empid;

                    $rstdsc     = $value['descricao'];
                    $rstdscprov = $value['providencia'];
//                    $rstitem    = ($value['tipo_pendencia'] == 'B') ? 'R' : $value['tipo_pendencia'] ;
                    $rstitem    = $value['tipo_pendencia'] ;
                    $dt_hj      = date('Y-m-d');
                    $data       = date('Y-m-d', strtotime($dt_hj . ' + 30 days'));

                    if($tipoChk == QUEID_QUEST_CHKLST_2P){
                        $tprid = 18;
                    }
                    elseif($tipoChk == QUEID_QUEST_CHKLST_ADM){
                        $tprid = 19;
                    }
                    elseif($tipoChk == QUEID_QUEST_CHKLST_TEC){
                        $tprid = 20; //Técnico
                    }
                    elseif($tipoChk == QUEID_QUEST_CHKLST_OBR_VINC){
                        $tprid = 22; //Obra Vinculada
                    }
                    else{
                        $tprid = 21; //Legado
                    }

                    switch ($estadoObra['esdid']) {
                        case '690': //Execução
                            $fsrid = 1;
                            break;
                        case '764': //Contratação
                            $fsrid = 4;
                            break;
                        case '691': //Paralisada
                            $fsrid = 5;
                            break;
                        case '763': //Licitação
                            $fsrid = 3;
                            break;
                        default:// Todos os Outros estados da Obra
                            $fsrid = 2;
                            break;
                    }

                    $dados = array('tprid'                      => $tprid,
                                   'fsrid'                      => $fsrid,
                                   'empid'                      => $empid,
                                   'rstdsc'                     => "'" . $rstdsc . "'",
                                   'rstdtprevisaoregularizacao' => "'" . $data . "'",
                                   'rstdscprovidencia'          => "'" . $rstdscprov . "'",
                                   'obrid'                      => $obrid,
                                   'rstitem'                    => "'".$rstitem."'",
                                   'rstflressalva'              => 'null',
                                   'rstobsressalva'             => 'null'
                                  );

                    $dados['rstdtinclusao'] = "now()";
                    $dados['rststatus']     = "'A'";
                    $dados['usucpf']        = "'" . $_SESSION['usucpf'] . "'";

                    $insert_campos = "";
                    $insert_valores = "";

                    foreach ($dados as $k => $v) {
                        if ($v == null) {
                            unset($dados[$k]);
                        } else {
                            $insert_campos .= $k . ",";
                            $insert_valores .= $v . ",";
                        }
                    }

                    $sql_insert = " INSERT INTO obras2.restricao
                                     (" . substr($insert_campos, 0, -1) . ") 
                                   VALUES 
                                   (" . substr($insert_valores, 0, -1) . ") 
                                   RETURNING rstid; ";

                    try {
                        $rstid = $this->pegaUm($sql_insert);
                        $restrict = new Restricao();
                        $docid = $restrict->atualizaDocidNullRetricao($rstid, true);
                    } catch (Exception $ex) {
                        $this->rollback();
                        $arr_return['erro'] = true;
                        $arr_return['msg'] .= '\nErro ao cadastrar a Restrição/Inconformidade. ' . $ex->getMessage();
                    }
                }
            }
        }
        return $arr_return;
    }

    /**
     * 
     * @param int $ckfid
     * @param array $array_dados
     * @param array $responsavel_destino
     */
    public function montaEnviaEmailInconformidadeChecklist2P($ckfid, $responsavel_destino = array()) {
        $email = new Email();
        $sql = 'SELECT o.obrid , o.obrnome 
                FROM obras2.obras o
                INNER JOIN obras2.checklistfnde ckf ON ckf.obrid = o.obrid
                WHERE ckf.ckfid = ' . $ckfid . ' 
                ';

        $arr_sup = $this->pegaLinha($sql);
        $nome_obra = '(' . $arr_sup['obrid'] . ')' . $arr_sup['obrnome'];
        $obrid = $arr_sup['obrid'];

        $dt = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        if (empty($responsavel_destino)) {
            $nome_responsavel  = 'Usuário não encontrado';
            $cpf_responsavel   = '000000';
            $email_responsavel = '';
        } else {
            $nome_responsavel  = $responsavel_destino['nome'];
            $cpf_responsavel   = $responsavel_destino['cpf'];
            $email_responsavel = $responsavel_destino['email'];
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
                                        <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        ' . $data . '
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
                                        <b> TRAMITAÇÃO DE CHECKLIST </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: OBRA ' . $nome_obra . ' - Validação de checklist da 2° parcela com pendência
                                    </td>
                                </tr>

                                <tr>
                                    <td colspan="2">
                                        Senhor prefeito, foram identificadas inconformidades no registro de informação da obra ' . $obrid . ', no preenchimento do sistema SIMEC, que podem impactar o repasse de recursos da referida obra. Solicitamos acessar o sistema e tomar as providências cabíveis.<br />
                                        Atenciosamente, 
                                    </td>
                                </tr>                 
                                <tr>
                                    <td colspan="2" style="text-align: center; padding: 10px 0 0 0;">
                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                        <br />
                                        <b>Fábio Lúcio de Almeida Cardoso<b>
                                        <br />
                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                        <br />
                                        CGIMP/DIRPE/FNDE/MEC
                                    </td>
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

        $assunto = "OBRA " . $nome_obra . " - Validação de checklist da 2° parcela com pendência";
        $conteudo = $html;

        $dados = array(
                        'usucpf'               => $_SESSION['usucpf'],
                        'emlconteudo'          => $conteudo,
                        'emlassunto'           => $assunto,
                        'temid'                => 15,
                        'emlregistroatividade' => true,
                        'obrid'                => $obrid
                      );

        foreach ($responsavel_destino as $key => $usuario) {
            $destinatario = array($usuario['email']);
            $email->popularDadosObjeto($dados);
            $email->salvar($destinatario);
            if (!empty($destinatario)) {
                $email->enviar();
            }
        }
    }
    /**
     * 
     * @param int $ckfid
     * @param array $array_dados
     * @param array $responsavel_destino
     */
    public function montaEnviaEmailInconformidadeChecklistAdm($ckfid, $responsavel_destino = array()) {
        $email = new Email();
        $sql = 'SELECT o.obrid , o.obrnome 
                FROM obras2.obras o
                INNER JOIN obras2.checklistfnde ckf ON ckf.obrid = o.obrid
                WHERE ckf.ckfid = ' . $ckfid . ' 
                ';

        $arr_sup = $this->pegaLinha($sql);
        $nome_obra = '(' . $arr_sup['obrid'] . ')' . $arr_sup['obrnome'];
        $obrid = $arr_sup['obrid'];

        $dt = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        if (empty($responsavel_destino)) {
            $nome_responsavel  = 'Usuário não encontrado';
            $cpf_responsavel   = '000000';
            $email_responsavel = '';
        } else {
            $nome_responsavel  = $responsavel_destino['nome'];
            $cpf_responsavel   = $responsavel_destino['cpf'];
            $email_responsavel = $responsavel_destino['email'];
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
                                        <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        ' . $data . '
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
                                        <b> TRAMITAÇÃO DE CHECKLIST </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: OBRA ' . $nome_obra . ' - Validação de checklist Administrativo com pendência
                                    </td>
                                </tr>

                                <tr>
                                    <td colspan="2">
                                        Senhor prefeito, foram identificadas inconformidades no registro de informação da obra ' . $obrid . ', no preenchimento do sistema SIMEC, que podem impactar o repasse de recursos da referida obra. Solicitamos acessar o sistema, na ABA Restrições e Inconformidades, e tomar as providências ali descritas para as inconformidades do tipo "Checklist Administrativo".<br />
                                        Atenciosamente, 
                                    </td>
                                </tr>                 
                                <tr>
                                    <td colspan="2" style="text-align: center; padding: 10px 0 0 0;">
                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                        <br />
                                        <b>Fábio Lúcio de Almeida Cardoso<b>
                                        <br />
                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                        <br />
                                        CGIMP/DIRPE/FNDE/MEC
                                    </td>
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

        $assunto = "OBRA " . $nome_obra . " - Validação de Checklist Administrativo com pendência";
        $conteudo = $html;

        $dados = array(
                        'usucpf'               => $_SESSION['usucpf'],
                        'emlconteudo'          => $conteudo,
                        'emlassunto'           => $assunto,
                        'temid'                => 15,
                        'emlregistroatividade' => true,
                        'obrid'                => $obrid
                      );

        foreach ($responsavel_destino as $key => $usuario) {
            $destinatario = array($usuario['email']);
            $email->popularDadosObjeto($dados);
            $email->salvar($destinatario);
            if (!empty($destinatario)) {
                $email->enviar();
            }
        }
    }
    /**
     * 
     * @param int $ckfid
     * @param array $array_dados
     * @param array $responsavel_destino
     */
    public function montaEnviaEmailInconformidadeChecklistTec($ckfid, $responsavel_destino = array()) {
        $email = new Email();
        $sql = 'SELECT o.obrid , o.obrnome 
                FROM obras2.obras o
                INNER JOIN obras2.checklistfnde ckf ON ckf.obrid = o.obrid
                WHERE ckf.ckfid = ' . $ckfid . ' 
                ';

        $arr_sup = $this->pegaLinha($sql);
        $nome_obra = '(' . $arr_sup['obrid'] . ')' . $arr_sup['obrnome'];
        $obrid = $arr_sup['obrid'];

        $dt = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        if (empty($responsavel_destino)) {
            $nome_responsavel  = 'Usuário não encontrado';
            $cpf_responsavel   = '000000';
            $email_responsavel = '';
        } else {
            $nome_responsavel  = $responsavel_destino['nome'];
            $cpf_responsavel   = $responsavel_destino['cpf'];
            $email_responsavel = $responsavel_destino['email'];
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
                                        <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        ' . $data . '
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
                                        <b> TRAMITAÇÃO DE CHECKLIST </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: OBRA ' . $nome_obra . ' - Validação de checklist Técnico com pendência
                                    </td>
                                </tr>

                                <tr>
                                    <td colspan="2">
                                        Senhor prefeito, foram identificadas inconformidades no registro de informação da obra ' . $obrid . ', no preenchimento do sistema SIMEC, que podem impactar o repasse de recursos da referida obra. Solicitamos acessar o sistema, na ABA Restrições e Inconformidades, e tomar as providências ali descritas para as inconformidades do tipo "Checklist Técnico".<br />
                                        Atenciosamente, 
                                    </td>
                                </tr>                 
                                <tr>
                                    <td colspan="2" style="text-align: center; padding: 10px 0 0 0;">
                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                        <br />
                                        <b>Fábio Lúcio de Almeida Cardoso<b>
                                        <br />
                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                        <br />
                                        CGIMP/DIRPE/FNDE/MEC
                                    </td>
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

        $assunto = "OBRA " . $nome_obra . " - Validação de Checklist Técnico com pendência";
        $conteudo = $html;

        $dados = array(
                        'usucpf'               => $_SESSION['usucpf'],
                        'emlconteudo'          => $conteudo,
                        'emlassunto'           => $assunto,
                        'temid'                => 15,
                        'emlregistroatividade' => true,
                        'obrid'                => $obrid
                      );

        foreach ($responsavel_destino as $key => $usuario) {
            $destinatario = array($usuario['email']);
            $email->popularDadosObjeto($dados);
            $email->salvar($destinatario);
            if (!empty($destinatario)) {
                $email->enviar();
            }
        }
    }
    /**
     * 
     * @param int $ckfid
     * @param array $array_dados
     * @param array $responsavel_destino
     */
    public function montaEnviaEmailInconformidadeChecklistObraVinculada($ckfid, $responsavel_destino = array()) {
        $email = new Email();
        $sql = 'SELECT o.obrid , o.obrnome 
                FROM obras2.obras o
                INNER JOIN obras2.checklistfnde ckf ON ckf.obrid = o.obrid
                WHERE ckf.ckfid = ' . $ckfid . ' 
                ';

        $arr_sup = $this->pegaLinha($sql);
        $nome_obra = '(' . $arr_sup['obrid'] . ')' . $arr_sup['obrnome'];
        $obrid = $arr_sup['obrid'];

        $dt = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        if (empty($responsavel_destino)) {
            $nome_responsavel  = 'Usuário não encontrado';
            $cpf_responsavel   = '000000';
            $email_responsavel = '';
        } else {
            $nome_responsavel  = $responsavel_destino['nome'];
            $cpf_responsavel   = $responsavel_destino['cpf'];
            $email_responsavel = $responsavel_destino['email'];
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
                                        <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                        ' . $data . '
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
                                        <b> TRAMITAÇÃO DE CHECKLIST </b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding:20px 0 20px 0;" colspan="2">
                                      Assunto: OBRA ' . $nome_obra . ' - Validação de checklist de Obra Vinculada com pendência
                                    </td>
                                </tr>

                                <tr>
                                    <td colspan="2">
                                        Senhor prefeito, foram identificadas inconformidades no registro de informação da obra ' . $obrid . ', no preenchimento do sistema SIMEC, que podem impactar o repasse de recursos da referida obra. Solicitamos acessar o sistema, na ABA Restrições e Inconformidades, e tomar as providências ali descritas para as inconformidades do tipo "Checklist de Obra Vinculada".<br />
                                        Atenciosamente, 
                                    </td>
                                </tr>                 
                                <tr>
                                    <td colspan="2" style="text-align: center; padding: 10px 0 0 0;">
                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                        <br />
                                        <b>Fábio Lúcio de Almeida Cardoso<b>
                                        <br />
                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                        <br />
                                        CGIMP/DIRPE/FNDE/MEC
                                    </td>
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

        $assunto = "OBRA " . $nome_obra . " - Validação de Checklist de Obra Vinculada com pendência";
        $conteudo = $html;

        $dados = array(
                        'usucpf'               => $_SESSION['usucpf'],
                        'emlconteudo'          => $conteudo,
                        'emlassunto'           => $assunto,
                        'temid'                => 15,
                        'emlregistroatividade' => true,
                        'obrid'                => $obrid
                      );

        foreach ($responsavel_destino as $key => $usuario) {
            $destinatario = array($usuario['email']);
            $email->popularDadosObjeto($dados);
            $email->salvar($destinatario);
            if (!empty($destinatario)) {
                $email->enviar();
            }
        }
    }
    
    public function liberaSegundaParcelaChecklist2pConcluido($ckfid) {
        $this->carregarPorId($ckfid);        
        $obrid                  = $this->obrid;
        $arCamposNulo           = array();
        $vldstatushomologacao 	= 'S' ;
        $usucpf_homo 		= $_SESSION['usucpf'] ;
        $vlddtinclusaosthomo	= "now()" ;
        $vldobshomologacao      = 'Homologado automaticamente via Checklist da Segunda Parcela.';

        $validacao = new Validacao();
        $dadosValidacao = $validacao->pegaValidacaoObra($obrid);

        if(!empty($dadosValidacao)){
            $validacao->popularDadosObjeto($dadosValidacao);
        } else {
            $arCamposNulo[] 	= 'vlddtinclusaost25exec';
            $arCamposNulo[] 	= 'vlddtinclusaost25exec';
            $arCamposNulo[] 	= 'usucpf_50';
            $arCamposNulo[] 	= 'vlddtinclusaost50exec';
            $arCamposNulo[] 	= 'vldobs25exec';
            $arCamposNulo[] 	= 'vldobs50exec';

            $validacao->vldstatus25exec 	= ' ';
            $validacao->vldstatus50exec 	= ' ';
        }

        $arDado = array(
            'obrid' 		=> $obrid,
            'usucpf_homo' 	        => $usucpf_homo,
            'vldstatushomologacao'  => $vldstatushomologacao,
            'vlddtinclusaosthomo'   => $vlddtinclusaosthomo,
            'vldobshomologacao' 	=> $vldobshomologacao
        );

        $validacao->popularDadosObjeto( $arDado )->salvar(true, true, $arCamposNulo);
        $validacao->commit();
    }
    
    public function gravaRegistroAtividadeChecklistConcluido($ckfid) {        
        $this->carregarPorId($ckfid);        
        //Recupera o tipo do Checklist
        $tipo = (int)$this->getTipoChecklistFnde($ckfid);        
        //Verifica se possui pendencias
        $fl_pendencias    = false;
        switch ($tipo) {
            case QUEID_QUEST_CHKLST_TEC:
                $tec = $this->montaArrayDadosPendenciaChecklistTec($ckfid);
                $fl_pendencias = (count($tec) > 0) ? true : false;
                break;
            case QUEID_QUEST_CHKLST_2P:
                $sp  = $this->montaArrayDadosPendenciaChecklist2P($ckfid);
                $fl_pendencias = (count($sp) > 0) ? true : false;
                break;
            case QUEID_QUEST_CHKLST_ADM:
                $adm = $this->montaArrayDadosPendenciaChecklistAdm($ckfid);
                $fl_pendencias = (count($adm) > 0) ? true : false;
                break;
            case QUEID_QUEST_CHKLST_OBR_VINC:
                $adm = $this->montaArrayDadosPendenciaChecklistObraVinculada($ckfid);
                $fl_pendencias = (count($adm) > 0) ? true : false;
                break;
            default:
                break;
        }
        
        if($fl_pendencias){
            $str_pendencias = ' com pendências.';
        }else{
            $str_pendencias = ' sem pendências.';
        }        
        // Recupera os dados
        $dadosChkLst = $this->getDados();
        
        switch ($tipo) {
            //TEC
            case QUEID_QUEST_CHKLST_TEC:
                $rgadscsimplificada = 'Checklist Técnico concluído. ID: '.$ckfid;
                $rgadsccompleta     = 'Checklist Técnico concluído '.$str_pendencias;
                break;
            //2 Parcela
            case QUEID_QUEST_CHKLST_2P:
                $rgadscsimplificada = 'Checklist da 2º parcela concluído. ID: '.$ckfid;
                $rgadsccompleta     = 'Checklist da 2º parcela concluído '.$str_pendencias;
                break;
            //ADM
            case QUEID_QUEST_CHKLST_ADM:
                $rgadscsimplificada = 'Checklist Administrativo concluído. ID: '.$ckfid;
                $rgadsccompleta     = 'Checklist Administrativo concluído '.$str_pendencias;
                break;
            // Obra Vinculada
            case QUEID_QUEST_CHKLST_OBR_VINC:
                $rgadscsimplificada = 'Checklist de Obra Vinculada concluído. ID: '.$ckfid;
                $rgadsccompleta     = 'Checklist de Obra Vinculada concluído '.$str_pendencias;
                break;
            default:
                $rgadscsimplificada = 'Checklist concluído. ID: '.$ckfid.' - Tipo(Tipo não utilizado mais): '.$tipo;
                $rgadsccompleta     = 'Checklist concluído '.$str_pendencias;
                break;
        }

        include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
        $regAtividade = new RegistroAtividade();
        $arDado       = array();

        $arDado['obrid']              = $this->obrid;
        $arDado['rgaautomatica']      = true;
        $arDado['rgadscsimplificada'] = $rgadscsimplificada;
        $arDado['rgadsccompleta']     = $rgadsccompleta;

        $arCamposNulo = array();

        $arCamposNulo[] = 'arqid'; 	
        
        $rga = $regAtividade->popularDadosObjeto( $arDado )
                            ->salvar(true, true, $arCamposNulo);
        $regAtividade->commit();
        
        return $rga;
        
    }
    
}