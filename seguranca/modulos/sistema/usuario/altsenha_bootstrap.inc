<?php

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

$urlLocation = str_replace($_SERVER['PHP_SELF'], '', $_SERVER['REQUEST_URI']);
//ver($_REQUEST, d);
if (!$_SESSION ['logincadastro']) {
    if ($_REQUEST ['act'] == 'alterar') {
        // checagem de consistencias
        $sql = "select ususenha from usuario where usucpf='" . $_SESSION ['usucpf'] . "'";
        $senha_banco = $db->recuperar($sql);
        $senha = md5_decrypt_senha($senha_banco ['ususenha'], '');

        if ($senha != $_REQUEST ['senhaatual']) {
            simec_redirecionar($urlLocation, 'warning', 'A senha atual não confere.');
            exit();
        }
    }
}

if (($_REQUEST['act'] == 'alterar') and ( !is_array($msgerro))) {
    # fazer alteração da senha.
    $sql = "update usuario set usuchaveativacao = 't', ususenha='" . md5_encrypt_senha($_REQUEST['novasenha'], '') . "' where usucpf='" . $_SESSION ['usucpf'] . "'";
    $saida = $db->executar($sql);
    $db->commit();
    unset($_SESSION['logincadastro']);
    unset($_SESSION['chave']);
    simec_redirecionar($urlLocation, 'success', 'Sua senha foi cadastrada com sucesso!');
}

include APPRAIZ . "includes/cabecalho.inc";
print '<br>';

if ($_SESSION ['logincadastro']) {
    simec_redirecionar($urlLocation, 'success', 'Sua senha foi cadastrada com sucesso! <br /><br /> Entretanto, é necessário que você troque-a no primeiro acesso.');
}

?>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Alteração de senha</li>
    </ol>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Alterar Senha Atual</h5>
                </div>
                <div class="ibox-content">
                    <form id="formAltSenha" name="formulario" class="form-horizontal" method="post">
                        <input type=hidden name="act" id="act" value=0>
                        <?php
                            if($_SESSION['logincadastro']):
                                echo '<input type=hidden name="senhaatual" value="OK">';
                            else:
                                echo $simec->input('senhaatual', 'Senha atual', $filtropi['senhaatual'], ['type' => 'password', 'size' => 20]);
                            endif;

                            echo $simec->input('novasenha', 'Nova senha', $filtropi['novasenha'], ['type' => 'password', 'size' => 20]);
                            echo $simec->input('confsenha', 'Confirma nova senha', $filtropi['confsenha'], ['type' => 'password', 'size' => 20]);
                        ?>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" onclick="validar_cadastro(); return false;">
                                    <span class="glyphicon glyphicon-saved"></span> Atualizar
                                </button>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    function validar_cadastro() {

        var mensagemValidacao = 'Os campos abaixo devem ser preenchidos: <br />';
        var controleValidacao = true;

        if( $('#senhaatual').val() == '' ){
             $("#divsenhaatual").removeClass('form-group').addClass('form-group has-error');
             mensagemValidacao += 'Informe a Senha Atual.<br />';
             controleValidacao = false;
        }else{
            $("#divsenhaatual").removeClass('form-group has-error').addClass('form-group');
        }

        if( $('#novasenha').val() == '' ){
            $("#divnovasenha").removeClass('form-group').addClass('form-group has-error');
            mensagemValidacao += 'Informe a Nova Senha.<br />';
            controleValidacao =  false;
        }else{
            $("#divnovasenha").addClass('form-group').removeClass('form-group has-error');
        }
        
        if( $('#confsenha').val() == '' ){
            $("#divconfsenha").removeClass('form-group').addClass('form-group has-error');
            mensagemValidacao += 'Confirma a nova senha.<br />';
            controleValidacao =  false;
        }else{
            $("#divconfsenha").addClass('form-group').removeClass('form-group has-error');
        }

        if( ($('#novasenha').val()) != '' && ($('#novasenha').val().length < 4) || ($('#novasenha').val().length > 12) ){
            mensagemValidacao += "A senha deve ter um mínimo de 4 caracteres e ter um máximo de 12 caracteres.<br />";
            controleValidacao =  false;
        }
        if( $('#novasenha').val() != $('#confsenha').val()){
            mensagemValidacao += 'A nova senha não confere com a segunda digitação.<br />';
            controleValidacao =  false;
        }

        if(controleValidacao){
            $('#act').val('alterar');
            $('#formAltSenha').submit();
        } else {
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

