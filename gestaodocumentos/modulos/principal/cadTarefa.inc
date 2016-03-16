<script type="text/javascript">
    /*function verificaSePermitido() {
        return '<?PHP echo $_SESSION['gt_doc']['perfilHabilitado']; ?>';
    }

    function verificaSeSuperUsuario() {
        return '<?PHP echo $_SESSION['gt_doc']['perfilSuperUsuario']; ?>';
    }*/
</script>

<?PHP
    require_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentos.class.inc";
    require_once APPRAIZ . "gestaodocumentos/classes/Acompanhamento.class.inc";
    require_once APPRAIZ . "gestaodocumentos/classes/PalavraChave.class.inc";
    require_once APPRAIZ . "includes/classes/dateTime.inc";

    //Verificar se existe tarefa
    if ($_GET['tarid']) {
        boExisteTarefa($_GET['tarid'], true);
    }
    
    //Ação Gravar
    if ($_POST['acaoForm']) {
        header('content-type: text/html; charset=ISO-8859-1');
        
        extract($_POST);
        $obTarefa = new GestaoDocumentos();

        //Inserção do campos "'tartiponumsidoc','tarnumsidoc'" no array
        $arCampos = array('tarid', 'tartitulo', 'tardsc', 'tartema', 'tmdid', 'tartiponumsidoc', 'taranobase', 'tarnumsidoc', 'tarprioridade', 'unaidsetororigem', 'unaidsetorresponsavel', 'sitid', 'tardepexterna', 'temid', 'nvcid', 'tpeid', 'tarnumprocexterno', 'tarsitprocexterno', 'tarnumidentexterno', 'tarsitarquivo' );

        $obTarefa->popularObjeto($arCampos);
        $obTarefa->tartipo = "T";

        $obData = new Data();
        if ($tardatarecebimento) {
            $obTarefa->tardatarecebimento = $obData->formataData($tardatarecebimento, "YYYY-mm-dd");
        }
        
        if($tmdid){
            $obTarefa->tmdid = $tmdid;
        } else {
            $obTarefa->tmdid = NULL;
        }
        
        if($temid){
            $obTarefa->temid = $temid;
        } else {
            $obTarefa->temid = NULL;
        }
        
        $perfil = pegarPerfil($_SESSION['usucpf']);
        
        if($usucpfresponsavel){
        	$obTarefa->usucpfresponsavel = $usucpfresponsavel;
        	$arCamposNulo = array();
        } elseif(empty($usucpfresponsavel) && !in_array(PERFIL_GDOCUMENTO_SUPER_USUARIO,$perfil)) {
        	$obTarefa->usucpfresponsavel = $_SESSION['usucpf'];
        	$arCamposNulo = array();
        } else {
        	$obTarefa->usucpfresponsavel = null;
        	$arCamposNulo = array('usucpfresponsavel');
        }
        
        if($obTarefa->tarid){
            $sql = "SELECT usucpfresponsavel, nvcid FROM gestaodocumentos.tarefa WHERE tarid = {$obTarefa->tarid}";
            $msg = $db->pegaLinha($sql);
        }
        if($usucpfresponsavel != $msg['usucpfresponsavel']){
            $obTarefa->tarcomplexidade = 't';
        } else {
            $obTarefa->tarcomplexidade = 'f';
        }        

        $obTarefa->tardatainicio = $obData->formataData($tardatainicio, "YYYY-mm-dd");
        $obTarefa->tardataprazoatendimento = $obData->formataData($tardataprazoatendimento, "YYYY-mm-dd");
        $obTarefa->tardataprazolinhadebase = $obData->formataData($tardataprazoatendimento, "YYYY-mm-dd");
        $obTarefa->tarnumsidoc = str_replace(".", "", str_replace("-", "", str_replace("/", "", $_POST['tarnumsidoc'])));
        $obTarefa->tartiponumsidoc = $_POST['tartiponumsidoc'];
        $obTarefa->tarporcentoexec = '10';

        if(empty($obTarefa->tarid)){
            $obTarefa->tarrecebimento = true;
        }
        
        if($nvcid != $msg['nvcid']){
        	$obTarefa->tarcomplexidade = 't';
        } else {
        	$obTarefa->tarcomplexidade = 'f';
        }
        
        if (!$obTarefa->tarid) {
            $sql = "SELECT MAX(_tarordem) FROM gestaodocumentos.tarefa WHERE _tarpai IS NULL";
            $max_tarordem = $db->pegaUm($sql);
            $tamanho = strlen($max_tarordem);
            $tamanho = ($tamanho) ? $tamanho : 4;
            $soma = intval($max_tarordem) + 1;
            $max_ordem = str_pad($soma, $tamanho, 0, STR_PAD_LEFT);
            $obTarefa->_tarordem = $max_ordem;
        }

        $obTarefa->salvar(true, true, $arCamposNulo);
        
        #VERIFICA E SE NÃO EXISTIR CRIA DOCID.
        $DOCID_tarid = $obTarefa->tarid;
        criaDocidGestaoDocumentos( $DOCID_tarid );
        
        if($acodsc){
            $obTarefa->salvarAcompanhamento($_POST, true);
        }
        
        #GRAVAR PALAVRA CHAVE.
        $objPalavraChave = new PalavraChave();
        
        if(isset($_POST['expressaochave'])){
            
            $a = $objPalavraChave->deletarPalavraPorTarid( $obTarefa->tarid );
            
            foreach ($_POST['expressaochave'] as $expressaochave){
                $arDados['plcdsc'] = $expressaochave;
                $arDados['tarid']  = $obTarefa->tarid;
                $objPalavraChave->popularDadosObjeto( $arDados )-> salvar();
                $objPalavraChave->clearDados();
            }
        } else {
            $a = $objPalavraChave->deletarPalavraPorTarid( $obTarefa->tarid );
        }
        $db->commit();
        
        $tarid = $obTarefa->tarid;
        
        $db->sucesso("principal/cadTarefa", "&tarid=" . $tarid);
        die;
    }

    if( $_GET['tarid'] != '' ){
        $tarid                  = $_GET['tarid'];
        $_SESSION['tarid']      = $_GET['tarid'];
        $_SESSION['_tartarefa'] = $_GET['tarid'];
    }
    $_SESSION['dados_tarefa']['tarid'] = $tarid;
    
    if ($tarid) {
        # Verifica se existe tarefa
        boExisteTarefa($tarid, true);
    }

    //OBJETO TAREFA
    $obTarefa = new GestaoDocumentos($tarid);
    if ($obTarefa->tarid) {
        $_SESSION['tarid'] = $obTarefa->tarid;
        $_SESSION['_tartarefa'] = $obTarefa->tarid;
    } elseif (isset($_SESSION['cadTarefa'])) {
        $arCadTarefa = $_SESSION['cadTarefa'];
        //inserção dos campos "'tartiponumsidoc','tarnumsidoc'" no array
        $arAtributos = array(
            'tarid' => null,
            'tartipo' => null,
            '_tartarefa' => null,
            '_tarpai' => null,
            '_tarordem' => null,
            '_tarprofundidade' => null,
            'tartitulo' => null,
            'tardsc' => null,
            'tartema' => null,
            'tmdid' => null,
            'tartiponumsidoc' => null,
            'tarnumsidoc' => null,
            'tarprioridade' => null,
            'unaidsetororigem' => null,
            'unaidsetorresponsavel' => null,
            'usucpfresponsavel' => null,
            'sitid' => null,
            'tardatarecebimento' => null,
            'tardatainicio' => null,
            'tardataprazolinhadebase' => null,
            'tardataprazoatendimento' => null,
            'tardataconclusao' => null,
            'tardepexterna' => null,
            'taranobase' => null,
            'tarporcentoexec' => null,
            'nvcid' => null, 
            'tpeid' => null
            
        );

        foreach ($arCadTarefa as $key => $value) {
            if (array_key_exists($key, $arAtributos)) {
                $obTarefa->$key = iconv("UTF-8", "ISO-8859-1", $value);
            }
            if ($key == 'acodsc') {
                $acodsc = iconv("UTF-8", "ISO-8859-1", $value);
            }
        }
        unset($_SESSION['cadTarefa']);
    }

    if (isset($_SESSION['iescodSession'])) {
        $instituicoesSelecionadas = implode(',', $_SESSION['iescodSession']);
    }
    
    if($_REQUEST['requisicao'] != '') {
        $_REQUEST['requisicao']($_REQUEST);
        die();
    }     
    
    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';
    
    $db->cria_aba($abacod_tela, $url, '');
    
    monta_titulo('Cadastar Demanda', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="./js/gestaodocumentos.js"></script>
<script type="text/javascript" src="./js/ajax.js"></script>

<center>
    <div id="aguarde_" style="display: none;position:absolute;color:#000033;top:50%;left:35%; width:300;font-size:12px;z-index:0;">
        <br><img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
    </div>
</center>

<form method="post" name="formulario" id="formulario" action="/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadTarefa&acao=A">
    <input type="hidden" name="acaoForm" id="acaoForm" value="1" />
    <input type="hidden" name="REQUEST_URI" id="REQUEST_URI" value="<?= $_SERVER['REQUEST_URI']; ?>" />
    <input type="hidden" name="cadTarefa" id="cadTarefa" value="1" />
    <input type="hidden" name="tipo" id="tipo" value="addSolicitante" />
    <input type="hidden" name="tarid" id="tarid" value="<? echo $obTarefa->tarid; ?>" />
    <input type="hidden" name="requisicao" id="requisicao" value="" />
    <input type="hidden" name="rtrid" id="rtrid" value="" />

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <?PHP
            echo blocoDadosTarefa($obTarefa, $arCadTarefa, $instituicoesSelecionadas);
            echo blocoDadosAtendimento($obTarefa, 'N', false, true, $acodsc, $arCadTarefa); 
        ?>
    </table>
    
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
        <tr>
            <td class="SubTituloDireita" colspan="2" style="text-align:center">
                <?PHP
                    $permissao = buscaPermissaoPerfilSalvar( $obTarefa->tarid );
                    if( $permissao == 'S' ){
                ?>
                        <input type="button" value="Salvar" onclick="enviaForm();"/>
                        <input type="button" value="Incluir Novo" onclick="novaTarefa();" />
                        <input type="button" value="Voltar para Listagem de Demandas" onclick="voltarListagem();" />
                <?PHP
                    }else{
                ?>
                        <input type="button" value="Salvar" disabled="disabled"/>
                        <input type="button" value="Incluir Novo"disabled="disabled"/>
                        <input type="button" value="Voltar para Listagem de Demandas" onclick="voltarListagem();" />
                <?PHP      
                    }
                ?>
                
            </td>
        </tr>
    </table>
    
</form>

<script type="text/javascript">
    function abrirPopupSolicitante( tarid ) {
        window.open('gestaodocumentos.php?modulo=principal/popupSolicitante&acao=A&tarid='+tarid, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=650');
    }

    function abrirPopupObjetoDemandante( tarid ) {
        window.open('gestaodocumentos.php?modulo=principal/popupObjetoDemanda&acao=A&tarid='+tarid, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=550');
    }

    function enviaForm() {
        var nomeform = 'formulario';
        var submeterForm = true;
        var campos = new Array();
        var tiposDeCampos = new Array();

        campos[0] = "tartitulo";
//        campos[1] = "tmdid";
//        campos[2] = "tartema";
        campos[3] = "tardatarecebimento";
        campos[4] = "unaidsetororigem";
        campos[5] = "unaidsetorresponsavel";
//        campos[6] = "tarprioridade";
//        campos[7] = "tardatainicio";
//        campos[8] = "tardataprazoatendimento";
//        campos[9] = "tardataprazolinhadebase";
//        campos[10] = "sitid";
        campos[11] = "tpeid";
//        campos[12] = "temid";
//        campos[13] = "tardsc";
//        campos[14] = "tartiponumsidoc";
        campos[15] = "tarsitprocexterno";
        campos[16] = "taranobase";
//        campos[17] = "usucpfresponsavel";
//        campos[18] = "nvcid";
//        campos[19] = "tarnumidentexterno";
//        campos[20] = "tarnumprocexterno";
//        campos[21] = "tarsitarquivo";

        tiposDeCampos[0] = "texto";
//        tiposDeCampos[1] = "select";
//        tiposDeCampos[2] = "texto";
        tiposDeCampos[3] = "data";
        tiposDeCampos[4] = "select";
        tiposDeCampos[5] = "select";
//        tiposDeCampos[6] = "radio";
//        tiposDeCampos[7] = "data";
//        tiposDeCampos[8] = "data";
//        tiposDeCampos[9] = "data";
//        tiposDeCampos[10] = "select";
        tiposDeCampos[11] = "select";
//        tiposDeCampos[12] = "select";
//        tiposDeCampos[13] = "texto";
        tiposDeCampos[14] = "radio";
        tiposDeCampos[15] = "texto";
        tiposDeCampos[16] = "texto";
//        tiposDeCampos[17] = "select";
//        tiposDeCampos[18] = "select";
//        tiposDeCampos[19] = "texto";
//        tiposDeCampos[20] = "texto";
//        tiposDeCampos[21] = "texto";

        validaForm(nomeform, campos, tiposDeCampos, submeterForm)
    }

    function habilitaCamposExterna( hab ){
        if(hab == 'S'){
            $('tarnumidentexterno').removeAttribute('disabled');
            $('tarnumprocexterno').removeAttribute('disabled');
            $('tarnumidentexterno').removeAttribute('readonly');
            $('tarnumprocexterno').removeAttribute('readonly');
            $('tarnumidentexterno').removeClassName('disabled');
            $('tarnumprocexterno').removeClassName('disabled');
            $('tarnumidentexterno').addClassName('normal');
            $('tarnumprocexterno').addClassName('normal');
        }else{
            $('tarnumidentexterno').disable();
            $('tarnumprocexterno').disable();
            $('tarnumidentexterno').removeClassName('normal');
            $('tarnumprocexterno').removeClassName('normal');
            $('tarnumidentexterno').addClassName('disabled');
            $('tarnumprocexterno').addClassName('disabled');
        }
    }

    function novaTarefa() {
        var data = 'tipo=novaTarefa';
        new Ajax.Request(
            'ajax.php',{
            method: 'post',
            parameters: data,
            asynchronous: false,
            onComplete: function(r){
                window.location.href = 'gestaodocumentos.php?modulo=principal/cadTarefa&acao=I';
            }
        });
    }
    
    function voltarListagem(){
        window.location.href = "/gestaodocumentos/gestaodocumentos.php?modulo=principal/listaTarefas&acao=A&vListFss=S";
    }
    
</script>

<div id="teste"></div>