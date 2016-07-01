<?php



if ($_POST['ajaxlatid']){
	
	
	$sql = "SELECT 
					 r.rsoid AS codigo,
					 u.usunome AS descricao
					FROM 
					 	atendefnde.responsavelsolicitacao r
						inner join seguranca.usuario u on u.usucpf = r.usucpf
					WHERE
					 r.latid = ".$_POST['ajaxlatid']." 
					ORDER BY 2";
	$db->monta_combo( 'rsoid', $sql, 'S', '-- Informe o responsável --', '', '', '', '', '', 'rsoid', '', $rsoid );
	echo obrigatorio();
				
	exit;
}

if ($_POST['submeter'] == '1'){
	
	$sql = "INSERT INTO atendefnde.solicitacao(ateid, rsoid, solassunto, soldescricao)
    				VALUES (".$_SESSION['ateid'].",
    						".$_POST['rsoid'].",
    						'".$_POST['solassunto']."', 
    						'".$_POST['soldescricao']."' 
    						) returning solid";
	$solid = $db->pegaUm($sql);
	$db->commit();
	
	die('<script>
		alert("Solicitação cadastrada de nº '.$solid.'\nAcompanhe o andamento da mesma na tela Listar Solicitações");
		location.href = "?modulo=principal/listaSolicitacao&acao=A";
		</script>');
	
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$menuAba = array(0 => array("id" => 1, "descricao" => "Lista de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaAtendimento&acao=A"),
			  1 => array("id" => 2, "descricao" => "Dados de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/dadosAtendimento&acao=A"),
			  2 => array("id" => 3, "descricao" => "Anexos de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/anexosAtendimento&acao=A"),
			  3 => array("id" => 4, "descricao" => "Lista de Solicitações", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaSolicitacao&acao=A"),
			  4 => array("id" => 5, "descricao" => "Cadastro de Solicitação", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/cadastraSolicitacao&acao=A")
		  	  );

echo montarAbasArray($menuAba, $_SERVER['REQUEST_URI']);

monta_titulo('Cadastro de Solicitação', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript">
function salvarSol(){
	var formulario = document.formulario;

	if (formulario.solassunto.value == ''){
		alert('O campo "Assunto" é obrigatório.');
		formulario.solassunto.focus();
		return false;
	}
	if (formulario.soldescricao.value == ''){
		alert('O campo "Descrição" é obrigatório.');
		formulario.soldescricao.focus();
		return false;
	}

	if (formulario.latid.value == ''){
		alert('O campo "Setor Responsável" é obrigatório.');
		formulario.latid.focus();
		return false;
	}
	if (formulario.rsoid.value == ''){
		alert('O campo "Responsável" é obrigatório.');
		formulario.rsoid.focus();
		return false;
	}
	
	document.getElementById('submeter').value = '1';
			
	formulario.submit();
	
}



function filtraResp(latid){

		var td_responsavel   = document.getElementById('td_responsavel');
	
		if(!latid) latid = '999999';
		
		var req = new Ajax.Request('atendefnde.php?modulo=principal/cadastraSolicitacao&acao=A', {
				        method:     'post',
				        parameters: '&ajaxlatid='+latid,
				        onComplete: function (res)
				        {
							td_responsavel.innerHTML = res.responseText;
				        }
					});


}

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="submeter" id="submeter" value="" />	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td width="35%" class="SubTituloDireita">Assunto:</td>
		<td><?= campo_texto( 'solassunto', 'S', 'S', '', 76, 1000, '', ''); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" valign="top">Descrição:</td>
		<td>
			<?= campo_textarea('soldescricao', 'S', 'S', '', 80, 5, 3000); ?>
		</td>
	</tr>										
	<tr>
		<td class="SubTituloDireita" valign="top">Setor Responsável:</td>	
		<td valign="top">
			<?php
			$sql = "SELECT
							latid AS codigo,
							latdescricao AS descricao
						FROM
							atendefnde.localatendimento
						WHERE
							latstatus = 'A'
						ORDER BY 2 ASC";
			$db->monta_combo( 'latid', $sql, 'S', '-- Informe o setor --', 'filtraResp', '', '', '', '', 'latid', '' );
			echo obrigatorio();
			?>	
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Responsável:</td>	
		<td id="td_responsavel">
			<?php 
				$sql = "SELECT 
								 r.rsoid AS codigo,
								 u.usunome AS descricao
								FROM 
								 	atendefnde.responsavelsolicitacao r
									inner join seguranca.usuario u on u.usucpf = r.usucpf
								WHERE
								 r.latid = 999999 
								ORDER BY 2";
				$db->monta_combo( 'rsoid', $sql, 'S', '-- Informe o responsável --', '', '', '', '', '', 'rsoid', '', $rsoid );
				echo obrigatorio();
			?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td align="center" colspan="2">
			<input type="button" name="btn_salvar" value="Salvar" onclick="javascript:salvarSol();"/>
			<input type="button" name="btn_cancelat" value="Cancelar" onclick="location.href='atendefnde.php?modulo=principal/cadastraSolicitacao&acao=A';"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>