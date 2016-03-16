<?php

$ateid = $_SESSION['ateid'];

if ($_POST['ajaxUf']){
	
	if($_POST['ajaxUf'] != 'XX'){
		echo "<b>Estado(s): {$_POST['ajaxUf']}</b><br>";
		$stAnd = "AND regcod in ('".str_replace(",","','",$_POST['ajaxUf'])."')";
	}
	
	$sql_combo = "SELECT
					muncod AS codigo,
					regcod ||' - '|| mundsc AS descricao
				FROM
					public.municipio
				Where munstatus = 'A'
				$stAnd
				ORDER BY 2 ASC;";
	
	$wherePesqMun = array(
				array(
						"codigo" 	=> "mundsc",
						"descricao" => "Município:",
						"numeric"	=> false
			   		  )
			   );
			   
	$sql = "select	b.muncod as codigo, b.regcod ||' - '|| b.mundsc as descricao
			from atendefnde.atendimentomunicipio a
			inner join public.municipio b on b.muncod = a.muncod
			Where a.ateid = ".$ateid."
			order by 2";
	$muncod = $db->carregar($sql);
			   
			   
	combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Município(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, null, $wherePesqMun, null, true, false, null, true);
	unset($muncod);
	echo obrigatorio();
				

	exit;
}


if ($_POST['submeter'] == '1'){
	
	$sql = "UPDATE atendefnde.atendimento
			SET ateassunto='".$_POST['ateassunto']."',
				atedescricao='".$_POST['atedescricao']."'
			WHERE ateid = ".$ateid;
	$db->executar($sql);
	
	if($ateid){
		
		//temas
		$sql = "DELETE FROM atendefnde.atendimentotema WHERE ateid = ".$ateid;
		$db->executar($sql);
		foreach($_POST['temid'] as $d){
			$sql = "INSERT INTO atendefnde.atendimentotema(ateid, temid) VALUES ($ateid, $d)";	
			$db->executar($sql);
		}
		
		unset($d);
		
		//estados
		$sql = "DELETE FROM atendefnde.atendimentouf WHERE ateid = ".$ateid;
		$db->executar($sql);
		foreach($_POST['uf'] as $d){
			$sql = "INSERT INTO atendefnde.atendimentouf(ateid, estuf) VALUES ($ateid, '{$d}')";	
			$db->executar($sql);
		}
		
		unset($d);
		
		//municipios
		$sql = "DELETE FROM atendefnde.atendimentomunicipio WHERE ateid = ".$ateid;
		$db->executar($sql);
		foreach($_POST['muncod'] as $d){
			$sql = "INSERT INTO atendefnde.atendimentomunicipio(ateid, muncod) VALUES ($ateid, '{$d}')";	
			$db->executar($sql);
		}
		
		unset($d);
		
		//solicitantes
		$sql = "DELETE FROM atendefnde.atendimentosolicitante WHERE ateid = ".$ateid;
		$db->executar($sql);
		foreach($_POST['solid'] as $d){
			$sql = "INSERT INTO atendefnde.atendimentosolicitante(ateid, solid) VALUES ($ateid, $d)";	
			$db->executar($sql);
		}
		
		
	}
	
	$db->commit();
	
	die('<script>
		alert("Operação efetuada com sucesso.");
		location.href = "?modulo=principal/dadosAtendimento&acao=A";
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

monta_titulo('Dados de Atendimento', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');


//recupera dados
$sql = "SELECT ateassunto, atedescricao
		FROM atendefnde.atendimento
		WHERE ateid = ".$ateid;
$dados = $db->pegaLinha($sql);
if($dados) extract($dados);

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
function salvarAtend(){
	var formulario = document.formulario;

	if (formulario.ateassunto.value == ''){
		alert('O campo "Assunto" é obrigatório.');
		formulario.ateassunto.focus();
		return false;
	}

	if (formulario.temid[0].value == ''){
		alert('O campo "Temas" é obrigatório.');
		formulario.temid.focus();
		return false;
	}
	if (formulario.uf[0].value == ''){
		alert('O campo "Estados" é obrigatório.');
		formulario.uf.focus();
		return false;
	}
	if (formulario.muncod[0].value == ''){
		alert('O campo "Municípios" é obrigatório.');
		formulario.muncod.focus();
		return false;
	}
	if (formulario.solid[0].value == ''){
		alert('O campo "Solicitantes" é obrigatório.');
		formulario.solid.focus();
		return false;
	}
	if (formulario.atedescricao.value == ''){
		alert('O campo "Descrição" é obrigatório.');
		formulario.atedescricao.focus();
		return false;
	}
	
 	selectAllOptions( formulario.temid );
	selectAllOptions( formulario.uf );
	selectAllOptions( formulario.muncod );
	selectAllOptions( formulario.solid );

	document.getElementById('submeter').value = '1';
			
	formulario.submit();
	
}



function filtraMunicipio(){

		var tr_municipio   = document.getElementById('tr_municipio');
		var td_municipio   = document.getElementById('td_municipio');

		var uf = document.getElementById('uf');
		var listauf = "";

		for(i=0;i<uf.length;i++){
			listauf = listauf + "," + uf[i].value;
		}

		if(listauf) listauf = listauf.substr(1);
		
		if(!listauf) listauf = 'XX';

		var req = new Ajax.Request('atendefnde.php?modulo=principal/dadosAtendimento&acao=A', {
				        method:     'post',
				        parameters: '&ajaxUf='+listauf,
				        onComplete: function (res)
				        {
							td_municipio.innerHTML = res.responseText;
				        }
					});

		
}

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="submeter" id="submeter" value="" />	

<?=cabecalhoAtendimento(); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td width="35%" class="SubTituloDireita">Assunto:</td>
		<td><?= campo_texto( 'ateassunto', 'S', 'S', '', 76, 1000, '', ''); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Temas:</td>	
		<td valign="top">
			<?php
				$sql_combo = "SELECT
						temid AS codigo,
						temdescricao AS descricao
					FROM
						atendefnde.tema
					WHERE
						temstatus = 'A'
					ORDER BY 2 ASC;";

				$wherePesqTema = array(
							array(
									"codigo" 	=> "temdescricao",
									"descricao" => "Tema:",
									"numeric"	=> false
						   		  )
						   );
						   
				$sql = "select	b.temid as codigo, b.temdescricao as descricao
						from atendefnde.atendimentotema a
						inner join atendefnde.tema b on b.temid = a.temid
						Where a.ateid = ".$ateid."
						order by 2";
				$temid = $db->carregar($sql);
						   
						   
				combo_popup( 'temid', $sql_combo, 'Selecione o(s) Tema(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, null, $wherePesqTema, null, true, false, null, true);
				echo obrigatorio();				
	    	?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Estados:</td>	
		<td valign="top">
			<?php
				$sql_combo = "SELECT
							regcod AS codigo,
							descricaouf AS descricao
						FROM
							public.uf
						WHERE idpais = 1
						ORDER BY 2 ASC;";
				
				$sql = "select	b.regcod as codigo, b.descricaouf as descricao
						from atendefnde.atendimentouf a
						inner join public.uf b on b.regcod = a.estuf
						Where b.idpais = 1
						and a.ateid = ".$ateid."
						order by 2";
				$ufm = $db->carregarColuna($sql);
				$uf = $db->carregar($sql);
								
				combo_popup( 'uf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', false, false, 4, 400, null, null, $dados_conexao, null, null, true, false, 'filtraMunicipio()', true);
				echo obrigatorio();
	    	?>		
		</td>
	</tr>
	<tr id="tr_municipio">
		<td class="SubTituloDireita" valign="top">Municípios:</td>	
		<td id="td_municipio">
			<?php 
			
				$sql_combo = "SELECT
								muncod AS codigo,
								regcod ||' - '|| mundsc AS descricao
							FROM
								public.municipio
							Where munstatus = 'A'
							and regcod = 'XX'
							ORDER BY 2 ASC;";

				$wherePesqMun = array(
							array(
									"codigo" 	=> "mundsc",
									"descricao" => "Município:",
									"numeric"	=> false
						   		  )
						   );
						   
				$sql = "select	b.muncod as codigo, b.regcod ||' - '|| b.mundsc as descricao
						from atendefnde.atendimentomunicipio a
						inner join public.municipio b on b.muncod = a.muncod
						Where a.ateid = ".$ateid."
						order by 2";
				$muncod = $db->carregar($sql);
						   
				
				if($ufm) echo "<b>Estado(s): ".implode(",",$ufm)."</b><br>";		   
				combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Município(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, null, $wherePesqMun, null, true, false, null, true);
				echo obrigatorio();
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Solicitantes:</td>	
		<td valign="top">
			<?php
				$sql_combo = "SELECT
							solid AS codigo,
							soldescricao AS descricao
						FROM
							atendefnde.solicitante
						WHERE
							solstatus = 'A'
						ORDER BY 2 ASC;";
				
				$wherePesqSol = array(
							array(
									"codigo" 	=> "soldescricao",
									"descricao" => "Solicitante:",
									"numeric"	=> false
						   		  )
						   );
						   
				$sql = "select	b.solid as codigo, b.soldescricao as descricao
						from atendefnde.atendimentosolicitante a
						inner join atendefnde.solicitante b on b.solid = a.solid
						Where a.ateid = ".$ateid."
						order by 2";
				$solid = $db->carregar($sql);
						   
						   
				combo_popup( 'solid', $sql_combo, 'Selecione o(s) Solicitante(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, null, $wherePesqSol, null, true, false, null, true);
				echo obrigatorio();
	    	?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Descrição:</td>
		<td>
			<?= campo_textarea('atedescricao', 'S', 'S', '', 80, 5, 3000); ?>
		</td>
	</tr>										
	<tr bgcolor="#C0C0C0">
		<td align="center" colspan="2">
			<input type="button" name="btn_salvar" value="Salvar" onclick="javascript:salvarAtend();"/>
			<input type="button" name="btn_cancelat" value="Cancelar" onclick="location.href='atendefnde.php?modulo=principal/dadosAtendimento&acao=A';"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>