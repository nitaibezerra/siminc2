<?
/*
 * Sistema Simec Setor responsável: SPO-MEC Desenvolvedor: 
 * Equipe Simec 
 * 
 * Analista: Werter Dias Almeida 
 * Programador: Lindalberto Filho 
 * Módulo:altsenha_bootstrap.inc 
 * Finalidade: permitir a alteração de senha com nova interface (bootstrap)
 * Data: 18/08/2014
 */

$modulo = "inicio"; 

if (! $_SESSION ['logincadastro']) {
    if ($_REQUEST ['act'] == 'alterar') {
        // checagem de consistencias
        $sql = "select ususenha from usuario where usucpf='" . $_SESSION ['usucpf'] . "'";
        $senha_banco = $db->recuperar ( $sql );
        $senha = md5_decrypt_senha ( $senha_banco ['ususenha'], '' );

        if ($senha != $_REQUEST ['senhaatual']) {?>
             <!--A senha atual não confere-->
            <script>
                alert ('A senha atual não confere.');
                history.back();
            </script>

            <? exit ();
        }
    }
}

if (($_REQUEST ['act'] == 'alterar') and (! is_array ( $msgerro ))) {
	// fazer alteração da senha.
	
	$sql = "update usuario set usuchaveativacao = 't', ususenha='" . md5_encrypt_senha ( $_REQUEST ['novasenha'], '' ) . "' where usucpf='" . $_SESSION ['usucpf'] . "'";
	$saida = $db->executar ( $sql );
	$db->commit ();
	unset ( $_SESSION ['logincadastro'] );
	unset ( $_SESSION ['chave'] );
	$db->sucesso ( $modulo );
        
}

include APPRAIZ . "includes/cabecalho.inc";
print '<br>';

if ($_SESSION ['logincadastro']) {
	print '<section class="alert alert-info col-md-6 col-md-offset-3 text-center">';
	print "<p>Seu cadastro foi realizado com sucesso!</p>";
	print "<p>Entretanto, é necessário que você troque-a no primeiro acesso.</p>";
	print '</section>';
	print '<br>';
}

?>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Alteração de senha</li>
    </ol>
    <script>
        function validar_cadastro() {

        var mensagemValidacao = 'Os campos abaixo devem ser preenchidos:\n';
        var controleValidacao = true;

        if( $('#senhaatual').val() == '' ){
             $("#divsenhaatual").removeClass('form-group').addClass('form-group has-error');
             mensagemValidacao += 'Informe a Senha Atual.\n';
             controleValidacao = false;
        }else{
            $("#divsenhaatual").removeClass('form-group has-error').addClass('form-group');
        }

        if( $('#novasenha').val() == '' ){
            $("#divnovasenha").removeClass('form-group').addClass('form-group has-error');
            mensagemValidacao += 'Informe a Nova Senha.\n';
            controleValidacao =  false;
        }else{
            $("#divnovasenha").addClass('form-group').removeClass('form-group has-error');
        }
        
        if( $('#confsenha').val() == '' ){
            $("#divconfsenha").removeClass('form-group').addClass('form-group has-error');
            mensagemValidacao += 'Confirma a nova senha.\n';
            controleValidacao =  false;
        }else{
            $("#divconfsenha").addClass('form-group').removeClass('form-group has-error');
        }

        if( ($('#novasenha').val()) != '' && ($('#novasenha').val().length < 4) || ($('#novasenha').val().length > 12) ){
            mensagemValidacao += "A senha deve ter um mínimo de 4 caracteres e ter um máximo de 12 caracteres.\n";
            controleValidacao =  false;
        }
        if( $('#novasenha').val() != $('#confsenha').val()){
                mensagemValidacao += 'A nova senha não confere com a segunda digitação.\n';
                controleValidacao =  false;
        }

       if(controleValidacao){
               $('#act').val('alterar');
               $('#formAltSenha').submit();
       }else{
               alert(mensagemValidacao);
               return false;
       }
}
	
    $(document).ready(function() {
 
	$("#senhaatual").bind({
		copy : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		},
		paste : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		},
		cut : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		}
	});
	
	$("#novasenha").bind({
		copy : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		},
		paste : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		},
		cut : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		}
	});
	
	$("#confsenha").bind({
		copy : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		},
		paste : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		},
		cut : function(){
			alert('Ao recuperar a senha, não copie e cole no navegador.');
			return false;
		}
	});
 
    });
    </script>
    <!--<title>Alteração de senha de Usuário</title>-->
    <div class="well col-md-12">
        <div class="container">
            <div class="col-lg-4 col-lg-offset-4">
                <form method="POST" name="formulario" id="formAltSenha" class="form form-horizontal">
                    <input type=hidden name="modulo" value="<?=$modulo?>"> 
                    <input type=hidden name="act" id="act" value=0>

                    <?if($_SESSION['logincadastro']){?>
                            <input type=hidden name="senhaatual" value="OK">
                    <?}?>	

                    <?if(!$_SESSION['logincadastro']){?>
                            <div class="form-group" id="divsenhaatual">
                            <label class="control-label col-lg-5" for="senhaatual"> Senha atual:</label>
                            <div class="col-lg-7"> 
                                <input type='password' class="form-control" name='senhaatual' id='senhaatual' size='20'>
                            </div>		 	        	
                    </div>			
                    <?}?>  
                            <div class="form-group" id="divnovasenha">
                            <label class="control-label col-lg-5" for="novasenha"> Nova senha:</label>
                            <div class="col-lg-7">
                                    <input type='password' class="form-control" name='novasenha' id='novasenha' size='20'>
                            </div>
                    </div>				 			 

                            <div class="form-group" id="divconfsenha">
                            <label class="control-label col-lg-5" for="confsenha"> Confirma nova senha:</label>
                            <div class="col-lg-7">
                                    <input type='password' class="form-control" name='confsenha' id='confsenha' size='20'>
                            </div> 
                    </div>
                    <div class="form-group">
                            <div class="col-lg-offset-5 col-lg-7" >
                                    <input type="button" class="botao btn btn-primary" name="btalterar"	value="Atualizar" onclick="validar_cadastro()"> <br> <br>
                            </div>						
                    </div>

                </form>
            </div>
        </div>
    </div>
