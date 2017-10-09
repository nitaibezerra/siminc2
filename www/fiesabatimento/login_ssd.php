<?php

if( $_SERVER['HTTP_HOST'] == 'simec-local' ){
	$_SESSION['sisbaselogin'] = 'simec_desenvolvimento';
	$_SESSION['baselogin'] = 'simec_desenvolvimento';
}elseif( $_SERVER['HTTP_HOST'] == 'simec-d.mec.gov.br' || $_SERVER['HTTP_HOST'] == 'simec-d' ){
	$_SESSION['sisbaselogin'] = 'simec_desenvolvimento';
	$_SESSION['baselogin'] = 'simec_desenvolvimento';
}else{
    
    
}

include_once '_constantes.php';

$_SESSION['mnuid'] = 12320;

function logarSSD($dados) {
	global $db;
	
	
	include_once '_funcoes.php';
	include_once '_constantes.php';

	// autenticar com SSD
	$permissao = true;

	if($permissao) {
		
		$perfil    = pegaPerfilFreire();
		
		switch($perfil['co_perfil']) {
			case '':
				$usucpf	= CPF_PROFESSOR;
				break;
			case '1':
				$usucpf	= CPF_PROFESSOR;
				break;
			/*	
			case '2':
				if( $perfil['co_dep_adm'] == 'F' ){
					$usucpf	= CPF_DIRETOR_FEDERAL;
				}elseif( $perfil['co_dep_adm'] == 'M' ){
					$usucpf	= CPF_DIRETOR_MUNICIPAL;
				}else{
					$usucpf	= CPF_DIRETOR_ESTADUAL;
				}
				break;
			case '3':
				$usucpf	= CPF_SECRETARIO;
				break;
			*/
			default:
				die("<script>alert('Usuário não possui perfil de professor na plataforma Freire.\\n\\Realize seu cadastro e preencha suas atuações no site http://freire.mec.gov.br/');window.location='http://".URL_SISTEMA."/fiesabatimento/login_ssd.php';</script>");
		}

		$sql = sprintf(
			"SELECT 
				u.usucpf, 
				u.ususenha, 
				u.suscod, 
				u.usutentativas, 
				u.usunome, 
				u.usuemail 
			 FROM 
				seguranca.usuario u
			 WHERE 
				u.usucpf = '%s'",
			$usucpf
		);
	
		$usuario = (object) $db->recuperar( $sql );
		
		unset( $usuario->ususenha );
		foreach ( $usuario as $attribute => $value ) {
			$_SESSION[$attribute] = $value;
		}
		
		$_SESSION['usucpforigem'] = $usucpf;

		// verifica permiss?o de acesso aos m?dulos
		$sql = sprintf(
			"SELECT
			s.sisid, s.sisdiretorio, s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel AS usunivel, us.susdataultacesso
			FROM seguranca.sistema s
			INNER JOIN seguranca.usuario_sistema us USING ( sisid )
			INNER JOIN seguranca.usuario u USING ( usucpf )
			INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
			INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod AND p.sisid = s.sisid
			WHERE
			us.suscod = 'A' AND
			u.usucpf = '%s' AND
			u.suscod = 'A' AND
			p.pflstatus = 'A'
			GROUP BY s.sisid, s.sisdiretorio,  s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel, us.susdataultacesso
			ORDER BY us.susdataultacesso DESC
			LIMIT 1",
			$usucpf
		);

		$sistema = (object) $db->pegaLinha( $sql );
		// carrega os dados do m?dulo para a sess?o
		foreach ( $sistema as $attribute => $value ) {
			$_SESSION[$attribute] = $value;
		}

//		ver($_SESSION,$db,d);
		header("location: fiesabatimento.php?modulo=inicio&acao=C");
		exit;
		

	}
	
}

date_default_timezone_set ('America/Sao_Paulo');

/* configura??es */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configura??es */
$_REQUEST['baselogin'] = 'simec_desenvolvimento';
// carrega as fun??es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// atribui??es requeridas para que a auditoria do sistema funcione
//$_SESSION['sisid'] = 4; # seleciona o sistema de segurança
$_SESSION['sisid'] = 133;
$_SESSION['sisbaselogin'] = 'simec_desenvolvimento';
$_SESSION['baselogin'] = 'simec_desenvolvimento';
$_SESSION['usucpf'] = '';
$_SESSION['usucpforigem'] = '';


// abre conex?o com o servidor de banco de dados
$db = new cls_banco();
//ver($_SESSION,d);
require_once APPRAIZ . 'includes/SSD.php';
if( $_GET['ok'] == 1 || $_GET['t'] != '' || $_SESSION['fiesabatimento_var']['t'] != '' ) {
	$_SESSION['fiesabatimento_var']['t'] = $_GET['t'] != '' ? $_GET['t'] : $_SESSION['fiesabatimento_var']['t'];
	$ssd = new Essencial_Adapter_SSD;
    
	if( empty($_SESSION['fiesabatimento_var']['t']) || is_null($_SESSION['fiesabatimento_var']['t']) ){
		header("location: http://".URL_SISTEMA."/fiesabatimento/login_ssd.php");
        exit;
	}
    
	$resposta = $ssd->retornarTicket($_SESSION['fiesabatimento_var']['t']);
    
    if(isset($resposta->message))
        echo "<script = lang='javascript'>alert('{$resposta->message}');history.go(0);<script>";
    
	$_SESSION['fiesabatimento_var']['cpfusuario'] = $resposta['dadosUsuario']->cpf;
	logarSSD($dados);
        exit;
}else{
//	$_SESSION['fiesabatimento_var']['cpfusuario'] = str_replace(array(".","-","/"),array("","",""),$dados['usucpf']);
//	$ssd = new Essencial_Adapter_SSD();
//	$solicitarAcesso = $ssd->solicitarLogin();
//	echo "<script>window.location = '{$solicitarAcesso}'</script>";
}
?>

<?php

include_once "config.inc";
include_once '_constantes.php';
include_once '_funcoes.php';

require_once APPRAIZ . 'includes/SSD.php';

$ssd = new Essencial_Adapter_SSD();
                
$urlAcessar = $ssd->solicitarAcesso(50);
$urlSolicitarAcesso = $ssd->solicitarLogin();
$urlAlterarDados = $ssd->solicitarAcesso(51);
$urlAlterarSenha = $ssd->solicitarAcesso(53);
$urlRecuperarEDesbloqueSenha = $ssd->solicitarAcesso(54);

?>
<?php
/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 24/08/2008
 */
//Verifica Temas

//if (isset($_COOKIE["theme_simec"])) {
//    $theme = $_COOKIE["theme_simec"];
//}
//
//if (isset($_POST["theme_simec"])) {
//    $theme = $_POST["theme_simec"];
//    setcookie("theme_simec", $_POST["theme_simec"], time() + 60 * 60 * 24 * 30, "/");
//}

// carrega as bibliotecas internas do sistema
//include "config.inc";
//require APPRAIZ . "includes/classes_simec.inc";
//include APPRAIZ . "includes/funcoes.inc";

// Valida o CPF, vindo do post
if ($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
    die('<script>
			alert(\'CPF inválido!\');
			history.go(-1);
		 </script>');
}

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// executa a rotina de autenticação quando o formulário for submetido
if ($_POST['formulario']) {
    if (AUTHSSD) {
        include APPRAIZ . "includes/autenticarssd.inc";
    } else {
        include APPRAIZ . "includes/autenticar.inc";
    }
}

if ($_REQUEST['expirou']) {
    $_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}


//Define um tema existente (padrão), caso nenhum tenha sido escolhido

$theme = 'verde';

//if (!$theme) {
//
//    $diretorio = APPRAIZ . "www/includes/layout";
//    if (is_dir($diretorio)) {
//        if ($handle = opendir($diretorio)) {
//            while (false !== ($file = readdir($handle))) {
//                if ($file != "." && $file != ".." && $file != ".svn" && is_dir($diretorio . "/" . $file)) {
//                    $dirs[] = $file;
//                }
//            }
//            closedir($handle);
//        }
//    }
//
//    if ($dirs) {
//        // sorteia um tema para exibição
//        $theme = $dirs[rand(0, (count($dirs) - 1))];
//        $_SESSION['theme_temp'] = $theme;
//    }
//}
?>
<!-- 
        Sistema Integrado de Monitoramento, Execução e Controle
        Setor responsvel: DTI/SE/MEC
        Finalidade: Tela de apresentação do sistema. Permite abrir uma sessão no sistema.
        Autor: Alexandre Dourado
-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="X-UA-Compatible" content="IE=7" />
        <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
            <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
            <script type="text/javascript" src="../../includes/funcoes.js"></script>
            <?php if (is_file("../includes/layout/" . $theme . "/include_login.inc")) include "../includes/layout/" . $theme . "/include_login.inc"; ?>
            <script type="text/javascript" src="../../includes/JQuery/jquery2.js"></script>
            <script type="text/javascript" src="../../includes/JQuery/jquery.accordion.source.js"></script>
            <script src="../../includes/BeautyTips/excanvas.js" type="text/javascript"></script>
            <script type="text/javascript" src="../../includes/BeautyTips/jquery.bt.min.js"></script>
            <style>
                a {text-decoration:none;}
            </style>
    </head>
    <body>
        <!--<div id="tutorial_theme" style="display:none"><span style="color:red;font-weight:bold;">Novidade!</span><br>Agora você pode escolher o VISUAL do seu SIMEC, clique no ícone ao lado e experimente!</div>-->
        <? include "../barragoverno.php"; ?>
        <table width="100%" cellpadding="0" cellspacing="0" id="main">
            <tr>
                <!--<td width="50%" ><img src="/includes/layout/<? echo $theme ?>/img/logo.png" border="0" /></td>-->
<!--                <td align="right" style="padding-right: 30px;padding-left:250px;" >
                    <img src="/includes/layout/<? echo $theme ?>/img/bt_temas.png" style="cursor:pointer" id="img_change_theme" alt="Alterar Tema" title="Alterar Tema" border="0" />
                    <div style="display:none" id="menu_theme">
                        <script>

                            $(document).ready(function() {
                                $().click(function() {
                                    $('#menu_theme').hide();
                                });
                                $("#img_change_theme").click(function() {
                                    $('#img_change_theme').btOff()
                                    $('#menu_theme').show();
                                    return false;
                                });
                                $("#menu_theme").click(function() {
                                    $('#menu_theme').show();
                                    return false;
                                });
                            });

                            function alteraTema() {
                                document.getElementById('formTheme').submit();
                            }
                        </script>

                        <form id="formTheme" action="" method="post" >

                            Tema: 
                            <select class="select_ylw" name="theme_simec" title="Tema do <?php echo SIGLA_SISTEMA; ?>" onchange="alteraTema(this.value)" >
                            <?php include(APPRAIZ . "www/listaTemas.php") ?>
                            </select>
                        </form>
                    </div>
                </td>-->
                <td height="25px"></td>
                <td></td>
            </tr>
            <tr>
                <td width="50%" valign="top" height="">
                    <!-- Lista de Módulos-->
                    <table width="98%" border="0" cellpadding="0" cellspacing="0" class="tabela_modulos">
                        <tr>
                            <td class="td_bg">&nbsp;SSD - Sistema de Segurança Digital</td>
                        </tr>
                        <tr>
                            <td valign="middle" class="td_table_inicio">
                                
                                <div>
                                    
                                <p align="left">Se você nunca acessou o FIES 1%, precisa soliciar acesso no SSD - Sistema de Segurança Digital, clicando no link abaixo.</p>
                                <p align="left">
                                    Mesmo que já tenha se cadastrado no SSD para acessar o sistema a Plataforma Freire é preciso  <b> clicar em "solicitar acesso" </b>
                                    <br /> para garantir sua autenticação no FIES 1%. A autorização será automática. 
                                    <br /> Uma vez autorizado o acesso, basta acessar clicando no link ao lado.
                                </p>
                                
<!--                            
                                <p align="center">Se você não possui cadastro no Sistema de Segurança Digital do Ministério da Educação (SSD)</p>

                                <p align="center">ou</p>

                                <p align="center">Se já possui cadastro no Sistema de Segurança Digital do Ministério da Educação (SSD) através de outros programas.</p>
-->

                                <p align="center"><b><a href="<?php echo $urlAcessar ?>">Solicitar Acesso</a></b></p>
                                <p><b>Outras Opções:</b></p>
                                <p><a href="<?php echo $urlAlterarDados ?>" >Alterar Dados</a></p>
                                <p><a href="<?php echo $urlAlterarSenha ?>" >Alterar Senha</a></p>
                                <p><a href="<?php echo $urlRecuperarEDesbloqueSenha ?>" >Recuperação e Desbloqueio de Senha</a></p>

                                </div>  
                            </td>

                        </tr>
                    </table>
                </td>
                <td width="50%" align="center" valign="top">
                    <table width="92%" border="0" align="center" cellpadding="0" cellspacing="0" class="tabela_modulos">
                        <tr>
                            <td class="td_bg">&nbsp;Acesso ao Fies Abatimento</td>
                        </tr>
                        <tr>
                            <td height="115">
                                <div>
                                    <p align="center">Se já realizou seu cadastro no SSD e já possui permissão de acesso.</p>
                                    <a href="<?php echo $urlSolicitarAcesso ?>" ><p align="center"><b>Clique aqui para Acessar</b></p></a>
                                </div>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr>
                <td colspan="2" class="rodape"> Data do Sistema: <? echo date("d/m/Y - H:i:s") ?></td>
            </tr>
        </table>
    </body>
</html>
<?php
// verificando se o browser é IE6 ou inferior
require APPRAIZ . "includes/classes/browser.class.inc";
$browser = new Browser();
if ($browser->getBrowser() == Browser::BROWSER_IE && $browser->getVersion() <= 6) {
    ?>
                                                                    <link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
                                                                    <script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
                                                                    <script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
                                                                    <script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
                                                                    <script>
                                /*** INICIO SHOW MODAL ***/

                                function montaShowModal() {
                                    var alert = '';
                                    alert += '<p align=center style=font-size:15;><font size=4 color=red><b>Atenção!</b></font><br>Seu navegador de internet está ultrapassado.<br/><br/>Em breve vamos descontinuar o suporte para Internet Explorer 6 e versões anteriores.<strong><br/><br/> Atualize seu navegador para obter uma experiência on-line mais rica, sugerimos algumas opções para download nos links abaixo:</strong></p>';
                                    alert += '<p><a target=_blank href=http://www.google.com/chrome/index.html?brand=CHNY&amp;utm_campaign=en&amp;utm_source=en-et-youtube&amp;utm_medium=et><img src=../imagens/browsers_chrome.png border=0></a> <a target=_blank href=http://www.microsoft.com/windows/internet-explorer/default.aspx><img src=../imagens/browsers_ie.png border=0></a> <a target=_blank href=http://www.mozilla.com/?from=sfx&amp;uid=267821&amp;t=449><img src=../imagens/browsers_firefox.png border=0></a></p>';
                                    alert += '<p align=center><input type=button value=Fechar onclick=closeMessage();></p>';
                                    displayStaticMessage(alert, false);
                                    return false;
                                }

                                function displayStaticMessage(messageContent, cssClass) {
                                    messageObj = new DHTML_modalMessage();	// We only create one object of this class
                                    messageObj.setShadowOffset(5);	// Large shadow

                                    messageObj.setHtmlContent(messageContent);
                                    messageObj.setSize(550, 280);
                                    messageObj.setCssClassMessageBox(cssClass);
                                    messageObj.setSource(false);	// no html source since we want to use a static message here.
                                    messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
                                    messageObj.display();
                                }

                                function closeMessage() {
                                    messageObj.close();
                                }

                                montaShowModal();
                                                                    </script>
    <?
}
?>


<script language="javascript">

    $('#img_change_theme').bt({
        trigger: 'none',
        contentSelector: "$('#tutorial_theme')",
        width: 200,
        shadow: true,
        shadowColor: 'rgba(0,0,0,.5)',
        shadowBlur: 8,
        shadowOffsetX: 4,
        shadowOffsetY: 4
    });

    $(document).ready(function() {
        $('#img_change_theme').btOn();
        window.setTimeout("$('#img_change_theme').btOff()", 10000);
    });


</script>	
