<?php

$visualiza = $_REQUEST['visualiza'];

$habil = $visualiza ? 'N' : 'S';

if( $_REQUEST['excluiTel'] ){
	if( $_REQUEST['telid'] ){
		$sql = "DELETE FROM siscap.telefone WHERE telid = ".$_REQUEST['telid'];
		$db->executar( $sql );
	}
	if($db->commit()){
		$db->sucesso( 'principal/gerenciaFacilitador', '&facid='.$_POST['facid'] );
	} else {
		echo "<script>
				alert('Falha na Operação');
				window.location.href = 'siscap.php?modulo=principal/formacao&acao=A&facid=".$_POST['facid'].";
			 </script>";
		exit();
	}
	//montaListaTelefone( $_REQUEST['facid'], $habil );
	exit;
}

if( $visualiza ){
	$titulo = 'Visualizar Facilitador';
} elseif( $_REQUEST['facid'] ){
	$titulo = 'Alterar Facilitador';	
} else {
	$titulo = 'Cadastrar Facilitador';	
}

require_once APPRAIZ . "www/includes/webservice/cpf.php";
include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo, '' );


if( $_POST['salva'] == 1 ){

	$faccpf = str_replace(array(".", "-"), "", $_POST['cpf']);
	
	if( $_POST['facid'] ){
		$facid = $_POST['facid'];
	} else {
		
		$sql = "SELECT count(faccpf) FROM siscap.facilitador WHERE faccpf = '".$faccpf."'";
		$cpf = $db->pegaUm( $sql );

		if( $cpf > 0 ){
			echo "<script>alert('Valor duplicado!');
				window.location.href='/siscap/siscap.php?modulo=principal/gerenciaFacilitador&acao=A'
				</script>";
			exit;
		}
		
		$sql = "INSERT INTO siscap.facilitador(
			            faccpf, facnome, facdtinc, facmincur, facconhe, facemail)
			    VALUES ('".$faccpf."', '".$_POST['nome']."', NOW(), '".$_POST['facmincur']."', '".$_POST['facconhe']."', '') RETURNING facid";
		$facid = $db->pegaUm( $sql );
	}

	if( $_POST['telnumero'] ){
		$tel = str_replace("-", "", $_POST['telnumero']);
		$sql = "INSERT INTO siscap.telefone( telnumero, facid, tteid, telddd ) VALUES ( '".$tel."', {$facid}, '".$_POST['tteid']."', '".$_POST['telddd']."' )";
		$db->executar( $sql );
	}
	
	$db->commit();
	echo "<script>alert('Operação realizada com sucesso');
		window.location.href='/siscap/siscap.php?modulo=principal/gerenciaFacilitador&acao=A&facid='+".$facid.";
		</script>";
}

if( $_REQUEST['facid'] ){
	$sql = "SELECT * FROM siscap.facilitador WHERE facid = ".$_REQUEST['facid'];
	$fac = $db->pegaLinha( $sql );
}

$visualiza = $_REQUEST['visualizar'];
$habilita = $visualiza == 1 ? 'N' : 'S';

$facnome = $fac['facnome'] ? $fac['facnome'] : $_REQUEST['facnome'];
?>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" id="curid" name="curid" value="<?=$_REQUEST['curid']?>" >
	<input type="hidden" name="nome" id="nome" value="<?=$facnome ?>" />
	<input type="hidden" name="facid" id="facid" value="<?=$_REQUEST['facid'] ?>" />
	<input type="hidden" name="salva" id="salva" value="" />
	<input type="hidden" name="excluiTel" id="excluiTel" value="" />
	<input type="hidden" name="telid" id="telid" value="" />
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width="15%" class="subtitulodireita">CPF:</td>
			<td colspan="3" width="35%">
				<?
					$faccpf = $fac['faccpf'] ? $fac['faccpf'] : $_REQUEST['faccpf'];
					$faccpf = formatar_cpf( $faccpf );
					//$habilita = $habil == 'N' ? 'disabled' : '';
					$habil = $habilita == 'N' ? 'disabled' : '';
				?>
				<input type='text' name='cpf' class='normal classcpf' <?=$habil ?> title='CPF' onkeyup='this.value=mascaraglobal("###.###.###-##", this.value);' class='CampoEstilo' id='cpf' value='<?=$faccpf ?>' size='17' maxlength='14' />
				<?=obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Nome do Facilitador:</td>
			<td id="recebeNome" colspan="3" width="35%">
				<label><?=$facnome ?></label>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Mini-Currículo:</td>
			<td colspan="3" width="35%">
				<?
				$facmincur = $fac['facmincur'] ? $fac['facmincur'] : $_REQUEST['facmincur'];
				echo campo_textarea("facmincur", 'S', $habilita, '', 85, 5, 800, '', '', '', '', "Mini-Currículo");
				?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Conhecimentos:</td>
			<td colspan="3" width="35%">
				<?
				$facconhe = $fac['facconhe'] ? $fac['facconhe'] : $_REQUEST['facconhe'];
				echo campo_textarea("facconhe", 'S', $habilita, '', 85, 5, 500, '', '', '', '', "Principais Conteúdos");
				?>
			</td>
		</tr>
	</table>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
	<tr>
		<td width="15%" class="subtitulodireita">Telefones:</td>
		<td colspan="3" width="35%">
			<?
			$sql = "SELECT tteid as codigo, ttedsc as descricao FROM siscap.tipo_telefone ORDER BY ttedsc";
			$arTipos = $db->carregar( $sql );
			$arTipos = $arTipos ? $arTipos : array();
			
			echo '<table width="50%" id="tb_tabela" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Ações</strong></td>
						<td align="Center" class="title" width="30%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Telefone</strong></td>
						<td align="Center" class="title" width="15%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Tipos de Telefones</strong></td>
					</tr>
				</thead>
				<tr>
					<td style="text-align: center;"><input align="center" type="button" id="btIncluir" value="Incluir" '.$habil.' onclick="salvar();" /></td>
					<td>'.campo_texto('telddd', 'N', $habilita, 'DDD', 3, 2, '', '', '', '', 0, 'id=telddd' )
					  	 .campo_texto('telnumero', 'S', $habilita, 'Número', 17, 9, '####-####', '', '', '', 0, 'id=telnumero' )."</td>
					<td><select id='tteid' style='width: 150px;' title='Tipos' class='CampoEstilo' name='tteid' ".$habil.">";
					  	 	foreach ($arTipos as $v) {
					  	 		echo '<option value="'.$v['codigo'].'">'.$v['descricao'].'</option>';
					  	 	}
						echo '</select>'.obrigatorio().'</td>
			    </tr>';

			if($_REQUEST['facid']){
				$sql = "SELECT
							t.telid as codigo,
							t.telnumero || '- ' || tp.ttedsc as descricao
						FROM
							siscap.telefone t
						INNER JOIN siscap.tipo_telefone tp ON tp.tteid = t.tteid
						WHERE
							facid = ".$_REQUEST['facid'];
				$tel = $db->carregar( $sql );
			}
				montaListaTelefone( $_REQUEST['facid'], $habilita );
				?>
			</table>
		</td>
	</tr>
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
	<tr bgcolor="#D0D0D0">
		<td colspan="4" style="text-align: center">			
			<?php if( $habilita == 'S' ){ ?>
				<input type="button" id="btSalvar" value="Salvar" onclick="salvar();" />
			<?php } ?>		
			<input type="button" id="btVoltar" value="Voltar" onclick="voltar();" />
		</td>
	</tr>
</table>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript">
	jQuery.noConflict();
	
	jQuery(document).ready(function(){
		jQuery('input.classcpf').live('change',function(){
			if( !validar_cpf( jQuery(this).val()  ) ){
				alert( "CPF inválido!\nFavor informar um cpf válido!" );
				jQuery(this).val('');
				return false;	
			}
			var comp  = new dCPF();
	
			comp.buscarDados( jQuery(this).val() );
			if (comp.dados.no_pessoa_rf != ''){
				jQuery('#recebeNome').html(comp.dados.no_pessoa_rf);
				jQuery('#nome').val(comp.dados.no_pessoa_rf);
			}
		});
	});
	

	form		=	$("formulario");
	btVoltar	=	$("btVoltar");
	
	function salvar(){
		btSalvar	=	$("btSalvar");
		btSalvar.disabled = true;
		btVoltar.disabled  = true;
		
		var camposObri 		= "#cpf#facmincur#facconhe";
		var tiposCamposObri	= '#texto#textarea#textarea';
		
		
		if(!validaForm('formulario',camposObri,tiposCamposObri,false)){
			btPesquisar.disabled = false;
			btCadastro.disabled  = false;
			return false;
		} else {
			document.getElementById('salva').value = 1;
			form.submit();
		}
	}
	function voltar(){
		history.back(-1);
	}
	function excluirTel( telid, facid ){
		if( confirm( "Deseja realmente excluir o telefone selecionado?" )){
			$('excluiTel').value = 'true';
			$('telid').value = telid;
			$('facid').value = facid;
			form.submit();
			/*var myAjax = new Ajax.Request('siscap.php?modulo=principal/gerenciaFacilitador&acao=A', {
				method 	   	 : 'post',
				parameters 	 : 'excluiTel=true&telid='+telid+'&facid='+facid,
				asynchronous : false,
				onComplete   : function(res){
					
					if(res.responseText)
					{
						alert("Operação realizada com sucesso!");
						//$('listaTelefones').innerHTML = res.responseText;
					}
					else
					{
						alert("Erro ao excluir o registro!");
					}
				}
			});*/
		}
	}
</script>