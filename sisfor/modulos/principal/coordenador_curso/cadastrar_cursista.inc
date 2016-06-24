<script src="/library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<?php

verificarFluxoComposicaoEquipe($_SESSION['sisfor']['docidcomposicaoequipe']);

if($_REQUEST['fpbid']){

//    $mensarios = recuperarCursista($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid']);
}

if($_REQUEST['envioForm']){
    if($_REQUEST['curnome'] && $_REQUEST['curemail'] && $_SESSION['sisfor']['sifid']){
    	
        if($_REQUEST['curcpf']){
            
	        $cpf = ereg_replace( "[^0-9]", "", $_REQUEST['curcpf']);
	
	        if($cpf) {
	        	
		        // Verifica se existe sisfor.cursista
		        $sql = "select curid, curemail from sisfor.cursista where curcpf = '{$cpf}'";
		        $cur = $db->pegaLinha($sql);
		
		        $curid = $cur['curid'];
	        
	        }
	        
        } else {
        	
            $cpf = "";
        }
        
        // Se não tiver sisfor.cursista ele insere. Se tiver, recupera e altera email, caso for.
        if(!$curid){
        	
            $sql = "insert into sisfor.cursista (curcpf, curnome, curemail)
                                                      values(".(($cpf)?"'".$cpf."'":"NULL").", '{$_REQUEST['curnome']}', '{$_REQUEST['curemail']}')
                    returning curid";
            
            $curid = $db->pegaUm($sql);
            
            $db->commit();
            
        } elseif($cur['curemail'] != $_REQUEST['curemail']) {
        	
            $sql = "update sisfor.cursista set
                        curemail = '{$_REQUEST['curemail']}'
                    where curid = '{$curid}'";
            
            $db->executar($sql);
            
        }

        // Insere ou altera sisfor.cursistacurso
        if($_REQUEST['cucid']){
        	
            $sql = "update sisfor.cursistacurso set
                        cucstatus = '{$_REQUEST['cucstatus']}',
                    where cucid = '{$_REQUEST['cucid']}'";

            $db->executar($sql);

        } else {
        	
        	$sql = "SELECT cucid FROM sisfor.cursistacurso WHERE curid='{$curid}' AND sifid='{$_SESSION['sisfor']['sifid']}' AND fpbid='{$_REQUEST['fpbid']}'";
        	$cucid = $db->pegaUm($sql);
        	
        	if(!$cucid) {
        	
	            $sql = "insert into sisfor.cursistacurso (curid, sifid, fpbid, estuf, muncod, iusdavaliador)
	                                            values('{$curid}', '{$_SESSION['sisfor']['sifid']}', '{$_REQUEST['fpbid']}', '{$_REQUEST['estuf']}', ".(($_REQUEST['muncod_nascimento'])?"'".$_REQUEST['muncod_nascimento']."'":"NULL").", '{$_REQUEST['iusdavaliador']}') returning cucid";
	            $cucid = $db->pegaUm($sql);
            
        	}
        }

        $db->commit();
    }
    die;
}

if('gravar' == $_REQUEST['acao']){

    if (isset($_REQUEST['avaliacao']) && is_array($_REQUEST['avaliacao'])) {
        foreach ($_REQUEST['avaliacao'] as $avaliacao) {
            $cucid = $avaliacao['cucid'] ? $avaliacao['cucid'] : null;
            $cavid = $avaliacao['cavid'] ? $avaliacao['cavid'] : null;
            
            if(!$cavid && $cucid) $cavid = $db->pegaUm("SELECT cavid FROM sisfor.cursistaavaliacoes WHERE cucid='".$cucid."'");
            
            $cavsituacao = $avaliacao['cavsituacao'] ? $avaliacao['cavsituacao'] : null;
            $cavparticipacao = ($avaliacao['cavparticipacao'] || $avaliacao['cavparticipacao'] === '0') ? str_replace(array('.', ','), array('', '.'), $avaliacao['cavparticipacao']) : "null";
            $cavatividadesrealizadas = ($avaliacao['cavatividadesrealizadas'] || $avaliacao['cavatividadesrealizadas'] === '0') ? str_replace(array('.', ','), array('', '.'), $avaliacao['cavatividadesrealizadas']) : "null";

            if($cavid){
            	
                $sql = " UPDATE sisfor.cursistaavaliacoes SET
                                cavparticipacao = $cavparticipacao,
                                cavatividadesrealizadas = $cavatividadesrealizadas,
                                cavsituacao = '$cavsituacao'
                         WHERE cavid = $cavid ";

            } else {
                $sql = " INSERT INTO sisfor.cursistaavaliacoes(cucid, cavparticipacao,cavatividadesrealizadas, cavsituacao)
                                VALUES ($cucid, $cavparticipacao, $cavatividadesrealizadas, '$cavsituacao') ";
            }

            $db->executar($sql);
            $db->commit();
        }
    }

    $location = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid={$_REQUEST['fpbid']}";
    $al = array("alert"=>"Operação realizada com sucesso!","location"=>$location);
    alertlocation($al);
}

monta_titulo('Cadastro de Cursistas','');

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td colspan="2" class="SubTituloCentro">
            <?
            carregarPeriodoReferenciaCursista(array('sifid'=>$_SESSION['sisfor']['sifid'], 'fpbid'=>$_REQUEST['fpbid']));
            ?>
        </td>
    </tr>
</table>

<? if($_REQUEST['fpbid']) : ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td colspan="2">
<p>Esta tela é destinada à inclusão de todos os cursistas matriculados no curso, a fim de que seja informada mensalmente a situação de cada um.</p> 
<p>Para cadastrar os cursistas, selecione o primeiro período de referência disponível, informe o CPF, e-mail ativo, estado e município em que atua e quem será o seu avaliador (dentre os bolsistas cadastrados na aba “Compor Equipe”). Em seguida, clique em "Inserir cursista". É obrigatório preencher todos os campos.</p>
<p>O sistema permite que o registro dos cursistas seja importado de uma planilha no formato “Comma Separated Values” ou “CSV”. É um formato de arquivo de texto utilizado para exportar dados de planilhas separando-os com vírgulas (ou outros tipos de separadores de campo).</p>
<p>Para importar os dados de uma planilha, clique no botão “Importar Cursistas via csv”, no lado direito da tela. O sistema solicitará novamente a indicação do período de referência. Em seguida, clique no botão “Selecionar arquivo”, assegurando-se de que o formato seja rigorosamente igual ao indicado pelo sistema. No campo relativo ao CPF do avaliador, só devem ser cadastrados os CPFs indicados na mesma tela, que são os CPFs da Equipe Pedagógica.</p> 
<p>Depois de selecionar o arquivo “csv”, clique em “Importar”. Observe os avisos do sistema quanto ao número de colunas e ao formato. Caso deseje utilizar esta função mas não tenha familiaridade com a extensão CSV, procure um tutorial na internet. Dúvidas  ou problemas com o sistema? Envie um e-mail para <?php echo $_SESSION['email_sistema']; ?>.</p>
        </td>
    </tr>
    <tr>
        <td colspan="2" class="SubTituloDireita">
            <a href='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=importar_cursista&fpbid=<?=$_REQUEST['fpbid'] ?>'><input type="button"  value="Importar Cursistas via csv"></a>
        </td>
    </tr>
</table>


<div id="div_cursista">
<?php montarFormularioCursista($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid']); ?>
</div>

<? endif; ?>

<script type="text/javascript">
    jQuery(function(){

        jQuery('.botao_enviar').live('click', function(){


            var valido = true;
            jQuery("#formulario_cursista input[type=text]").each(function(i, obj){
                if(!jQuery(obj).val() && jQuery(obj).attr('name') != "curcpf"){
                    valido = false;
                }
            });

            if(!valido){
                alert('Favor preencher todos os campos.');
                return false
            }

            if(!validaEmail(jQuery('#curemail').val())) {
            	alert('Email Principal inválido');
            	return false;
            }

            options = {
                success : function() {
                    jQuery("#div_cursista").load('/sisfor/ajax.php?atualizarFormularioCursista=1&sifid='+'<?php echo $_SESSION['sisfor']['sifid'] . '&fpbid=' .$_REQUEST['fpbid']?>');
                }
            }

            if(jQuery('#iusdavaliador').val()=='') {
				alert('Selecione um avaliador');
				return false;
            }

            if(jQuery('#estuf').val()=='') {
				alert('Selecione UF');
				return false;
            }

			if(document.getElementById('muncod_nascimento')) {
				
	            if(jQuery('#muncod_nascimento').val()=='') {
					alert('Selecione um município');
					return false;
	            }
	            
			} else {
				alert('Selecione UF');
				return false;
			}
            
            $form = jQuery("#formulario_cursista");
            $form.ajaxForm(options);
            $form.submit();
        });

        jQuery('.carregar_dados').live('click', function(){
            var url = jQuery(this).attr('href');
            var pflcod = jQuery(this).attr('pflcod');

            jQuery("#div_equipe_"+pflcod).load(url);
            return false;
        });

        jQuery('.deleta_tipo_perfil').live('click', function(){

            if(confirm('Deseja realmente excluir o registro?')){
                var url = jQuery(this).attr('href');
                var pflcod = jQuery(this).attr('pflcod');

                jQuery("#div_equipe_"+pflcod).load(url);
            }
            return false;
        });

        jQuery('#curcpf').live('blur', function(){

            if(jQuery(this).val()){
                jQuery.ajax({
                    url: '/sisfor/ajax.php?recuperarUsuario=1&iuscpf='+jQuery(this).val(),
                    dataType: 'json',
                    success: function(dados){
                        if(dados.nome){
                            jQuery('#curnome').val(dados.nome);
                            jQuery('#curemail').val(dados.email);
                        } else {
                            alert('O CPF informado é inválido.');
                            jQuery('#curnome, #curcpf, #curemail').val('');
                        }
                    }
                });
            }
        });
    });
</script>