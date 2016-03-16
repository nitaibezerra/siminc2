<?php
	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Orion Teles <orionteles@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Permite que o usuário redefina a senha
	 * Última modificação: 04/12/2014
	 */

	function erro(){
		global $db;
//		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'] = func_get_args();
        header( "Location: /login.php" );
        exit();
	}

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

    $theme = $_SESSION['theme_temp'];

	// executa a rotina de recuperação de senha quando o formulário for submetido
	if ( $_POST['formulario'] ) {

        $sql = " select *, (NOW() between ssedatasolicitacao and (ssedatasolicitacao + INTERVAL '2 DAYS')) valido
                        from seguranca.solicitacaosenha sse
                        where sseid = '{$_POST['sseid']}'
                        ;";

        $dados = $db->pegaLinha($sql);
        if ( $_POST['ususenha'] !=  $_POST['ususenhaconf']) {
            $_SESSION['MSG_AVISO'][] = "A confirmação da senha não combina com a senha informada.";
            header( "Location: /redefinir_senha.php?chave={$dados['ssehash']}" );
            exit();
        }

        if ( $_POST['ssehash'] !=  $dados['ssehash']) {
            $_SESSION['MSG_AVISO'][] = "A chave de acesso é inválida";
            header( "Location: /login.php" );
            exit();
        }

        if ( $dados['valido'] != 't') {
            $_SESSION['MSG_AVISO'][] = "A chave de acesso expirou. Favor solicitar outra.";
            header( "Location: /login.php" );
            exit();
        }


		// verifica se a conta está ativa
		$sql = sprintf(
			"SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = '%s'",
			corrige_cpf( $dados['usucpf'] )
		);
		$usuario = (object) $db->pegaLinha( $sql );
		if ( $usuario->suscod != 'A' ) {
			erro( "A conta não está ativa." );
		}

		$sql = sprintf(
			"UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 't' WHERE usucpf = '%s'",
			md5_encrypt_senha( $_POST['ususenha'], '' ),
			$usuario->usucpf
		);
		$db->executar( $sql );

		$sql = sprintf(
			"UPDATE seguranca.solicitacaosenha SET ssedatauso = '%s', sseipacesso = '%s' WHERE sseid = '%s'",
			date('Y-m-d H:i:s'),
            $_SERVER['REMOTE_ADDR'],
            $dados['sseid']
		);
		$db->executar( $sql );

		$db->commit();

		$_SESSION = array();
		$_SESSION['MSG_AVISO'][] = "Senha redefinida con sucesso.";
        header( "Location: /login.php" );
        exit();
	}
?>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="X-UA-Compatible" content="IE=7" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />

        <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

        <!-- Styles Boostrap -->
        <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
        <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
        <link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">

        <!-- Custom Style -->
        <link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
        <link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">
        <link href="estrutura/temas/default/css/estilon.css" rel="stylesheet">



        <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
        <!--[if lt IE 9]>
            <script src="estrutura/js/html5shiv.js"></script>
        <![endif]-->
        <!--[if IE]>
            <link href="estrutura/temas/default/css/styleie.css" rel="stylesheet">
        <![endif]-->

        <!-- Boostrap Scripts -->
        <script src="library/jquery/jquery-1.10.2.js"></script>
        <script src="library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
        <script src="library/chosen-1.0.0/chosen.jquery.min.js"></script>
        <script src="library/bootstrap-switch/js/bootstrap-switch.min.js"></script>

        <!-- Custom Scripts -->
        <script type="text/javascript" src="../includes/funcoes.js"></script>

        <script type="text/javascript">
            function ImprimeStatus(texto){
                document.formul.numCaracteres.value = texto
            }
            $(function(){
                $('.chosen-select').chosen();
            });
        </script>

        <style type="text/css">
            a.chosen-single{
                height: 30px !important;
                padding-top: 2px !important;
            }

            .switch-left, .switch-right{
                padding: 3px 10px !important
            }
        </style>

        <style type="text/css">
            td b, td p, td li{
                margin: 10px;

            }

            table tr td{
                padding: 3px;

            }

        </style>
        
        <style>
			#barra-brasil .brasil-flag {
				height: 100% !important;
			}
		</style>
	
    </head>

    <body>

       <? //include "barragoverno_2014.php"; ?>
       
       <div id="barra-brasil" style="background:#7F7F7F; height: 20px; padding:0 0 0 10px;display:block;"> 
			<ul id="menu-barra-temp" style="list-style:none;">
				<li style="display:inline; float:left;padding-right:10px; margin-right:10px; border-right:1px solid #EDEDED"><a href="http://brasil.gov.br" style="font-family:sans,sans-serif; text-decoration:none; color:white;">Portal do Governo Brasileiro</a></li> 
				<li><a style="font-family:sans,sans-serif; text-decoration:none; color:white;" href="http://epwg.governoeletronico.gov.br/barra/atualize.html">Atualize sua Barra de Governo</a></li>
			</ul>
		</div>
		
		<br>

        <div class="row">
            <div class="col-md-12">
                <img src="estrutura/temas/default/img/logo-simec.png">
            </div><!-- / .col-md-12 -->
        </div>
       <? if ( $_SESSION['MSG_AVISO'] ): ?>
           <div class="alert alert-danger">
               <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
               <?= implode( "<br />", (array) $_SESSION['MSG_AVISO'] ); ?>
           </div>
       <? endif;
       $_SESSION['MSG_AVISO'] = array(); ?>
<table width="100%" cellpadding="0" cellspacing="0" id="main">
	<input type=hidden name="formulario" value="1"/>
	<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
<tr>
      <td colspan="2" align="center" valign="top">
      <table width="98%"  border="0" align="center" cellpadding="0" cellspacing="0" class="tabela_modulos">
        <tr>
          <td class="td_bg">&nbsp;Ficha de Solicitação de Cadastro de Usuários</td>
        </tr>
        <tr>
          <td height="106" align="left">

           <?php

               $chave = base64_decode($_REQUEST['chave']);
               $dados = explode(':', $chave);

               $sql = " select *, (NOW() between ssedatasolicitacao and (ssedatasolicitacao + INTERVAL '2 DAYS')) valido
                        from seguranca.solicitacaosenha sse
                        where ssehash = '{$_GET['chave']}'
                        and ssedatauso is null";

               $dados = $db->pegaLinha($sql);

               if($dados['sseid']){
                   if('t' == $dados['valido']){ ?>
                       <form method="post" name="formulario">
                           <input type="hidden" name="sseid" value="<?php echo $dados['sseid']; ?>" />
                           <input type="hidden" name="ssehash" value="<?php echo $dados['ssehash']; ?>" />
                           <input type="hidden" name="formulario" value="1" />

                           <!--Caixa de Login-->
                           <table border="0" cellspacing="0" cellpadding="3">
                               <tr>
                                   <td style="font-weight: bold;text-align: right;width:150px">CPF:</td>
                                   <td style="width:250px" >
                                       <?php echo formatar_cpf($dados['usucpf']); ?>
                                   </td>
                               </tr>
                               <tr>
                                   <td style="font-weight: bold;text-align: right;width:150px">Nova Senha:</td>
                                   <td style="width:250px" >
                                       <input type="password" name="ususenha" class="login_input" />
                                       <?= obrigatorio(); ?>
                                   </td>
                               </tr>
                               <tr>
                                   <td style="font-weight: bold;text-align: right;width:150px">Confirme a Senha:</td>
                                   <td style="width:250px" >
                                       <input type="password" name="ususenhaconf" class="login_input" />
                                       <?= obrigatorio(); ?>
                                   </td>
                               </tr>
                               <tr>
                                   <td>&nbsp;</td>
                                   <td>
                                       <a class="botao2" href="javascript:enviar_formulario()" >Continuar</a>
                                       <a class="botao1" href="./login.php" >Voltar</a>
                                   </td>
                               </tr>
                           </table>
                           <!--fim Caixa de Login -->
                       </form>
                   <?php } else {
                        echo '<h1 style="font-size: 15pt; color: red; text-align: center; margin-top: 20px;">A chave de acesso expirou! Favor solicitar novamente.</h1>
                              <a class="botao1" href="./login.php" >Voltar</a>';
                   }
               } else {
                    echo '<h1 style="font-size: 15pt; color: red; text-align: center; margin-top: 20px;">A chave de acesso não é válida!</h1>
                          <a class="botao1" href="./login.php" >Voltar</a>';
               }
           ?>

          </td>

        </tr>
      </table>
      </td>
  </tr>

	<tr>
	  <td colspan="2" class="rodape"> Data do Sistema: <? echo date("d/m/Y - H:i:s") ?></td>
  </tr>
</table>


 <!-- Fim barra governo -->
 <script src="//static00.mec.gov.br/barragoverno/barra.js" type="text/javascript"></script>

</body>
</html>

<script language="javascript">

	document.formulario.ususenha.focus();

	function enviar_formulario() {

		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( !document.formulario.ususenha.value ) {
			mensagem += '\nO campo "Nova Senha" é de preenchimento obrigatório.';
			validacao = false;
		}
		if ( !document.formulario.ususenhaconf.value ) {
			mensagem += '\nO campo "Confirme a Senha" é de preenchimento obrigatório.';
			validacao = false;
		}
		if ( validacao && document.formulario.ususenhaconf.value != document.formulario.ususenha.value ) {
			mensagem = 'A confirmação da senha não combina com a senha informada';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

</script>