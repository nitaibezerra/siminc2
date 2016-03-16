<html>
    <head>
        <meta http-equiv="Cache-Control" content="no-cache">
        <meta http-equiv="Pragma" content="no-cache">
        <meta http-equiv="Connection" content="Keep-Alive">
        <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
        
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        
        <script language="JavaScript" src="../includes/calendario.js"></script>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/prototype.js"></script>
        <script type="text/javascript" src="../includes/entidades.js"></script>
        <script type="text/javascript" src="/includes/estouvivo.js"></script>
        
        <title>Dados do Dirigente - Módulo Acadêmico</title>
        
    </head>
    
    <body>
        <div>

<?PHP
    require_once APPRAIZ . "includes/classes/entidades.class.inc";

    switch($_REQUEST['acao']) {
        case 'A';
            $_FUNCOESAUTORIZADAS = array('salvarRegistroDirigente'=>true, 'salvarFormacaoDirigentes'=>true);
            break;
    }

    if($_REQUEST['funid']){
        $_SESSION['academico']['funid'] = $_REQUEST['funid'];
    }
    
    if( $_REQUEST['opc'] ){
        $_SESSION['academico']['opc'] = $_REQUEST['opc'];
    }
    
    if( $_REQUEST['sit'] ){
        $_SESSION['academico']['sit'] = $_REQUEST['sit'];
    }
    
    if( $_REQUEST['requisicao'] ){
        $_REQUEST['funid']  = $_SESSION['academico']['funid'];
        $_REQUEST['opc']    = $_SESSION['academico']['opc'];
        $_REQUEST['sit']    = $_SESSION['academico']['sit'];
        
        if( $_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']] ) {
            $_REQUEST['requisicao']($_REQUEST);
        }
    }
    
    $abacod_tela    = ABAS_DADOS_ADICIONAIS_DIRIGENTES;
    $url            = 'academico.php?modulo=principal/editardirigente&acao=A';
    $parametros     = "&entid={$_REQUEST['entid']}&funid={$_REQUEST['funid']}&opc={$_REQUEST['opc']}&sit={$_REQUEST['sit']}";
    
    $db->cria_aba( $abacod_tela, $url, $parametros );
    
    /* Verifica se o dirigente esta vinculado a uma função, caso não, imprimir erro */
    if($_SESSION['academico']['funid']) {
        $entidade = new Entidades();
        $gravar = true;
        if(permissaoInterna(array(PERFIL_DIRETOR_CAMPUS,PERFIL_INTERLOCUTOR_CAMPUS)) && $_SESSION['academico']['entidadenivel'] != 'campus'){
            $gravar = false;
        }

        if( $_REQUEST['opc'] == 'ED' ){
            $entidade->carregarPorEntid($_REQUEST['entid']);
        }
        
        if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
            $entid = $_SESSION['academico']['entid'];
        }else{
            $entid = $_SESSION['academico']['entidcampus'];
        }
        
        echo $entidade->formEntidade(
            "academico.php?modulo=principal/editardirigente&acao=A&requisicao=salvarRegistroDirigente&tela={$_REQUEST['tela']}",
            array( "funid" => $_SESSION['academico']['funid'], "entidassociado" => $entid ),
            array( "enderecos"=>array(1),"gravar" => $gravar)
        );
    }
?>
        </div>
    </body>
</html>    

<script type="text/javascript">
    $('frmEntidade').onsubmit  = function(e) {
        alert('ops');
        if (trim($F('entnumcpfcnpj')) == '') {
            alert('CPF é obrigatório.');
            return false;
        }
        if (trim($F('entnome')) == '') {
            alert('O nome da entidade é obrigatório.');
            return false;
        }
        if (trim($F('entdatanasc')) != '') {
            if(!validaData(document.getElementById('entdatanasc'))) {
                alert("Data de nascimento é inválida.");
                return false;
            }
        }
        if (trim($F('entemail')) == '') {
            alert("E-mail é obrigatório.");return false;
        }
        return true;
    }
</script>