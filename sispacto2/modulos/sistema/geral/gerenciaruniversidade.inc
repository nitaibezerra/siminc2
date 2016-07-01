<?
include APPRAIZ ."includes/workflow.php";
include_once "_funcoes.php";

function pegaUniversidadeJson($dados) {
	global $db;
	
	
	$sql = "SELECT uu.*, rr.* FROM sispacto2.universidadecadastro u 
			LEFT JOIN sispacto2.universidade uu ON uu.uniid = u.uniid 
			LEFT JOIN sispacto2.reitor rr ON rr.uniid = u.uniid 
			WHERE u.uncid='".$dados['uncid']."'";
	
	$arr = $db->pegaLinha($sql);

	ob_clean();
	
	echo simec_json_encode($arr);
	

}

function excluirUniversidade($dados) {
	global $db;

	$uniid = $db->pegaUm("SELECT uniid FROM sispacto2.universidadecadastro WHERE uncid='".$dados['uncid']."'");

	$sql = "DELETE FROM sispacto2.universidadecadastro WHERE uncid='".$dados['uncid']."'";
	$db->executar($sql);

	$sql = "DELETE FROM sispacto2.reitor WHERE uniid='".$uniid."'";
	$db->executar($sql);

	$sql = "DELETE FROM sispacto2.universidade WHERE uniid='".$uniid."'";
	$db->executar($sql);

	$db->commit();

	$al = array("alert"=>"Dados da universidade/reitor foram removidos com sucesso","location"=>"sispacto2.php?modulo=sistema/geral/gerenciaruniversidade&acao=A");
	alertlocation($al);

}

function gravarUniversidade($dados) {
	global $db;
	
	if($dados['uncid']) {
		
		$uniid = $db->pegaUm("SELECT uniid FROM sispacto2.universidadecadastro WHERE uncid='".$dados['uncid']."'");
		$mundescricao = $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$dados['muncod']."'");
		
		$sql = "UPDATE sispacto2.universidade SET 
						unisigla='".$dados['unisigla']."', 
						uninome='".$dados['uninome']."', 
						unicnpj='".str_replace(array(".","/","-"),array("","",""),$dados['unicnpj'])."', 
						unicep='".str_replace(array("-"),array(""),$dados['unicep'])."', 
						unimunicipio='".$mundescricao."', 
						unilogradouro='".$dados['unilogradouro']."',
			            unibairro='".$dados['unibairro']."', 
						unidddcomercial='".$dados['unidddcomercial']."', 
						uninumcomercial='".str_replace(array("-"),array(""),$dados['uninumcomercial'])."',
			            uniemail='".$dados['uniemail']."', 
						uniuf='".$dados['uniuf']."', 
						uninumero='".$dados['uninumero']."', 
						unisite='".$dados['unisite']."', 
						muncod='".$dados['muncod']."' 
				WHERE uniid='".$uniid."'";
		
		$db->executar($sql);
		
		$sql = "UPDATE sispacto2.reitor SET 
						reinome='".$dados['reinome']."', 
						reicpf='".str_replace(array(".","-"),array("",""),$dados['reicpf'])."', 
						reidddcomercial='".$dados['reidddcomercial']."', 
						reinumcomercial='".str_replace(array("-"),array(""),$dados['reinumcomercial'])."', 
            			reiemail='".$dados['reiemail']."' 
				WHERE uniid='".$uniid."'";
		
		$db->executar($sql);
		
		$db->commit();
		
	} else {
		
		$uniid = $db->pegaUm("SELECT (max(uniid)+1) as uniid FROM sispacto2.universidade");
		$mundescricao = $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$dados['muncod']."'");
		
		
		$sql = "INSERT INTO sispacto2.universidade(
			            uniid, 
						unisigla, 
						uninome, 
						unicnpj, 
						unicep, 
						unimunicipio, 
						unilogradouro,
			            unibairro, 
						unicomplemento, 
						unidddcomercial, uninumcomercial,
			            uniemail, 
						unistatus, 
						uniuf, 
						uninumero, 
						unisite, 
						muncod, 
						iesid)
			    VALUES (".$uniid.", 
			    		'".$dados['unisigla']."', 
			    		'".$dados['uninome']."', 
			    		'".str_replace(array(".","/","-"),array("","",""),$dados['unicnpj'])."', 
			    		'".str_replace(array("-"),array(""),$dados['unicep'])."', 
			    		'{$mundescricao}', 
			    		'".$dados['unilogradouro']."',
			            '".$dados['unibairro']."', 
			            null, 
			            '".$dados['unidddcomercial']."', 
			            '".str_replace(array("-"),array(""),$dados['uninumcomercial'])."',
			            '".$dados['uniemail']."', 
			            'A', 
			            '".$dados['uniuf']."', 
			            '".$dados['uninumero']."', 
			            '".$dados['unisite']."', 
			            '".$dados['muncod']."', 
			            null) returning uniid;";	

		$db->executar($sql);
		
		$reiid = $db->pegaUm("SELECT (max(reiid)+1) as reiid FROM sispacto2.reitor");
		
		$sql = "INSERT INTO sispacto2.reitor(
            			reiid, 
						uniid, 
						reinome, 
						reicpf, 
						reidddcomercial, 
						reinumcomercial, 
            			reiemail, 
						reistatus)
			    VALUES (".$reiid.", 
			    		".$uniid.", 
			    		'".$dados['reinome']."', 
			    		'".str_replace(array(".","-"),array("",""),$dados['reicpf'])."', '".$dados['reidddcomercial']."', 
			    		'".str_replace(array("-"),array(""),$dados['reinumcomercial'])."', 
			            '".$dados['reiemail']."', 
			            'A');";
		
		$db->executar($sql);
		
		$uncid = $db->pegaUm("SELECT (max(uncid)+1) as reiid FROM sispacto2.universidadecadastro");
		
		$sql = "INSERT INTO sispacto2.universidadecadastro(
            	uncid, curid, uncstatus, uniid, cadastrosgb)
    			VALUES (".$uncid.", 162, 'A', ".$uniid.", false);";
		
		$db->executar($sql);
		
		$db->commit();
		
		
	}
	

	$al = array("alert"=>"Dados da universidade/reitor foram salvos com sucesso","location"=>"sispacto2.php?modulo=sistema/geral/gerenciaruniversidade&acao=A");
	alertlocation($al);

}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>';

echo "<br>";

monta_titulo( "Lista - Universidades", "Gerenciando o cadastro das universidades");

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>

function gerenciarGestorEscola(lemcodigoinep) {
	window.open('sispacto2.php?modulo=principal/escola/gerenciargestorescola&acao=A&lemcodigoinep='+lemcodigoinep,'Gestor','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');	
}

function gerenciarApEscola(inep, obj) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	
	if(obj.title=="mais") {
		obj.title      = "menos";
		obj.src        = "../imagens/menos.gif";
		var nlinha     = tabela.insertRow(linha.rowIndex);
		var ncol0       = nlinha.insertCell(0);
		ncol0.innerHTML = "&nbsp;";
		var ncol1       = nlinha.insertCell(1);
		ncol1.colSpan   = 10;
		ncol1.id      = 'esc_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarCadastramentoEscola&lemcodigoinep='+inep,ncol1.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
	
}

function validarCadastramentoEscola(docid) {
	var conf = confirm('Deseja realmente validar o cadastramento da escola?');

	if(conf) {
		ajaxatualizar('requisicao=validarCadastramentoEscola&docid='+docid,'');
		jQuery('#formulario').submit();
	}
}


function mostrarUniversidade(uncid) {

	limparFormulario();

	if(uncid!='') {
		
		jQuery("#uncid").val(uncid);

        jQuery.getJSON('sispacto2.php?modulo=sistema/geral/gerenciaruniversidade&acao=A&requisicao=pegaUniversidadeJson&uncid='+uncid, function(data) {
            jQuery('#unisigla').val(data.unisigla);
            jQuery('#uninome').val(data.uninome);
            jQuery('#unicnpj').val(data.unicnpj);
            jQuery('#unicnpj').keyup();
            jQuery('#unicep').val(data.unicep);
            jQuery('#unicep').keyup();

            jQuery('#uniuf').val(data.uniuf);
            jQuery('#uniuf').change();
            jQuery('#muncod').val(data.muncod);
            jQuery('#unilogradouro').val(data.unilogradouro);
            jQuery('#unibairro').val(data.unibairro);
            jQuery('#uninumero').val(data.uninumero);
            jQuery('#uninumero').keyup();
            jQuery('#unidddcomercial').val(data.unidddcomercial);
            jQuery('#uninumcomercial').val(data.uninumcomercial);
            jQuery('#uninumcomercial').keyup();
            jQuery('#uniemail').val(data.uniemail);
            jQuery('#unisite').val(data.unisite);


            jQuery('#reicpf').val(data.reicpf);
            jQuery('#reicpf').keyup();
            jQuery('#reinome').val(data.reinome);
            jQuery('#reidddcomercial').val(data.reidddcomercial);
            jQuery('#reinumcomercial').val(data.reinumcomercial);
            jQuery('#reinumcomercial').keyup();
            jQuery('#reiemail').val(data.reiemail);

        });

	}

	jQuery("#modalUniversidade").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 750,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function excluirUniversidade(uncid) {
	var conf = confirm('Deseja realmente excluir esta universidade?');

	if(conf) {
		window.location='sispacto2.php?modulo=sistema/geral/gerenciaruniversidade&acao=A&requisicao=excluirUniversidade&uncid='+uncid;
	}
	
}


function limparFormulario() {
	jQuery('#unisigla').val('');
	jQuery('#uninome').val('');
	jQuery('#unicnpj').val('');
	jQuery('#unicep').val('');
	jQuery('#uniuf').val('');
	jQuery('#unilogradouro').val('');
	jQuery('#unibairro').val('');
	jQuery('#uninumero').val('');
	jQuery('#unidddcomercial').val('');
	jQuery('#uninumcomercial').val('');
	jQuery('#uniemail').val('');
	jQuery('#unisite').val('');
	jQuery('#reicpf').val('');
	jQuery('#reinome').val('');
	jQuery('#reidddcomercial').val('');
	jQuery('#reinumcomercial').val('');
	jQuery('#reiemail').val('');
	jQuery('#td_municipio').html('Selecione uma UF');
}


function gravarUniversidade() {
	
	if(jQuery('#unisigla').val()=='') {
		alert('Sigla precisa ser preenchida');
		return false;
	}

	if(jQuery('#uninome').val()=='') {
		alert('Universidade precisa ser preenchida');
		return false;
	}

	if(jQuery('#unicnpj').val()=='') {
		alert('CNPJ precisa ser preenchida');
		return false;
	}

	if(jQuery('#unicep').val()=='') {
		alert('CEP precisa ser preenchida');
		return false;
	}

	if(jQuery('#uniuf').val()=='') {
		alert('UF precisa ser preenchida');
		return false;
	}

	if(document.getElementById("muncod")) {
		
		if(jQuery('#muncod').val()=='') {
			alert('Município precisa ser selecionado');
			return false;
		}
		
	} else {
		
		alert('Selecione uma UF');
		return false;
		
	}

	if(jQuery('#unilogradouro').val()=='') {
		alert('Logradouro precisa ser preenchida');
		return false;
	}


	if(jQuery('#unibairro').val()=='') {
		alert('Bairro precisa ser preenchida');
		return false;
	}

	if(jQuery('#uninumero').val()=='') {
		alert('Número precisa ser preenchida');
		return false;
	}

	if(jQuery('#unidddcomercial').val()=='') {
		alert('DDD Universidade precisa ser preenchida');
		return false;
	}

	if(jQuery('#uninumcomercial').val()=='') {
		alert('Telefone Universidade precisa ser preenchida');
		return false;
	}


	if(jQuery('#uniemail').val()=='') {
		alert('Email da Universidade precisa ser preenchida');
		return false;
	}

	if(jQuery('#unisite').val()=='') {
		alert('Site da Universidade precisa ser preenchida');
		return false;
	}

	if(jQuery('#reicpf').val()=='') {
		alert('CPF do reitor precisa ser preenchida');
		return false;
	}

	if(jQuery('#reinome').val()=='') {
		alert('Nome do reitor precisa ser preenchido');
		return false;
	}

	if(jQuery('#reidddcomercial').val()=='') {
		alert('DDD reitor precisa ser preenchido');
		return false;
	}

	if(jQuery('#reinumcomercial').val()=='') {
		alert('Telefone reitor precisa ser preenchido');
		return false;
	}

	if(jQuery('#reiemail').val()=='') {
		alert('Email precisa ser preenchido');
		return false;
	}

	jQuery('#formulario').submit();
	
}

function carregarMunicipiosPorUF(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod&name=muncod&estuf='+estuf,'td_municipio');
	} else {
		document.getElementById('td_municipio').innerHTML = "Selecione uma UF";
	}
}

</script>

<div id="modalUniversidade" style="display:none;">
<form method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="gravarUniversidade">
<input type="hidden" name="uncid" id="uncid">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Dados da universidade</td>
</tr>
<tr>
	<td class="SubTituloDireita">Sigla</td>
	<td><? echo campo_texto('unisigla', "S", "S", "Sigla", 10, 50, "", "", '', '', 0, 'id="unisigla"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Universidade</td>
	<td><? echo campo_texto('uninome', "S", "S", "Universidade", 67, 150, "", "", '', '', 0, 'id="uninome"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">CNPJ</td>
	<td><? echo campo_texto('unicnpj', "S", "S", "CNPJ", 19, 19, "##.###.###/####-##", "", '', '', 0, 'id="unicnpj"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">CEP</td>
	<td><? echo campo_texto('unicep', "S", "S", "CEP", 10, 10, "#####-###", "", '', '', 0, 'id="unicep"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><? 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('uniuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF', '', '', '', 'S', 'uniuf', '');
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio"><?
	
		echo "Selecione uma UF";

	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Logradouro</td>
	<td><? echo campo_textarea( 'unilogradouro', 'S', 'S', '', '70', '4', '200'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Bairro</td>
	<td><? echo campo_texto('unibairro', "S", "S", "Bairro", 67, 150, "", "", '', '', 0, 'id="unibairro"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Número</td>
	<td><? echo campo_texto('uninumero', "S", "S", "Número", 6, 5, "#####", "", '', '', 0, 'id="uninumero"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Telefone</td>
	<td>(<? echo campo_texto('unidddcomercial', "N", "S", "DDD", 2, 3, "##", "", '', '', 0, 'id="unidddcomercial"' ); ?>) <? echo campo_texto('uninumcomercial', "S", "S", "Telefone", 10, 10, "####-####", "", '', '', 0, 'id="uninumcomercial"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">E-mail</td>
	<td><? echo campo_texto('uniemail', "S", "S", "E-mail", 67, 150, "", "", '', '', 0, 'id="uniemail"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Site</td>
	<td><? echo campo_texto('unisite', "S", "S", "Site", 67, 150, "", "", '', '', 0, 'id="unisite"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Dados do reitor</td>
</tr>
<tr>
	<td class="SubTituloDireita">CPF</td>
	<td><? echo campo_texto('reicpf', "S", "S", "CPF", 15, 15, "###.###.###-##", "", '', '', 0, 'id="reicpf"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome do reitor</td>
	<td><? echo campo_texto('reinome', "S", "S", "Nome do reitor", 67, 150, "", "", '', '', 0, 'id="reinome"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Telefone</td>
	<td>(<? echo campo_texto('reidddcomercial', "N", "S", "DDD", 2, 3, "##", "", '', '', 0, 'id="reidddcomercial"' ); ?>) <? echo campo_texto('reinumcomercial', "S", "S", "Telefone", 10, 10, "####-####", "", '', '', 0, 'id="reinumcomercial"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">E-mail</td>
	<td><? echo campo_texto('reiemail', "S", "S", "E-mail", 67, 150, "", "", '', '', 0, 'id="reiemail"' ); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Gravar" onclick="gravarUniversidade();"></td>
</tr>
</table>
</form>
</div>


<p><input type="button" name="inseriruniversidade" value="Inserir universidade" onclick="mostrarUniversidade('');"></p>
<br>
<?

$sql = "SELECT '<img src=../imagens/alterar.gif style=\"cursor:pointer;\" onclick=\"mostrarUniversidade(\''||u.uncid||'\');\"> <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirUniversidade('||u.uncid||')\">' as acao, unisigla, uninome, m.estuf, m.mundescricao 
		FROM sispacto2.universidadecadastro u 
		INNER JOIN sispacto2.universidade uu ON uu.uniid = u.uniid 
		INNER JOIN territorios.municipio m ON m.muncod = uu.muncod";

$cabecalho = array("&nbsp;","Sigla","Universidade","UF","Município");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

?>