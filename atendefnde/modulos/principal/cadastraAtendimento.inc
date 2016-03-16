<?php


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
			   
	combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Município(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, null, $wherePesqMun, null, true, false, null, true);
	echo obrigatorio();
				

	exit;
}


if ($_POST['submeter'] == '1'){
	
	$sql = "INSERT INTO atendefnde.atendimento(ateassunto, atedescricao, atedatainclusao, usucpf, atestatus)
    				VALUES ('".$_POST['ateassunto']."', 
    						'".$_POST['atedescricao']."', 
    						'".date("Y-m-d")."', 
    						'".$_SESSION['usucpf']."', 
    						'A') returning ateid";
	$ateid = $db->pegaUm($sql);
	
	if($ateid){
		//temas
		foreach($_POST['temid'] as $d){
			$sql = "INSERT INTO atendefnde.atendimentotema(ateid, temid) VALUES ($ateid, $d)";	
			$db->executar($sql);
		}
		
		unset($d);
		
		//estados
		foreach($_POST['uf'] as $d){
			$sql = "INSERT INTO atendefnde.atendimentouf(ateid, estuf) VALUES ($ateid, '{$d}')";	
			$db->executar($sql);
		}
		
		unset($d);
		
		//municipios
		foreach($_POST['muncod'] as $d){
			$sql = "INSERT INTO atendefnde.atendimentomunicipio(ateid, muncod) VALUES ($ateid, '{$d}')";	
			$db->executar($sql);
		}
		
		unset($d);
		
		//solicitantes
		foreach($_POST['solid'] as $d){
			$sql = "INSERT INTO atendefnde.atendimentosolicitante(ateid, solid) VALUES ($ateid, $d)";	
			$db->executar($sql);
		}
		
		
	}
	
	$db->commit();
	
	die('<script>
		alert("Atendimento cadastrado de nº '.$ateid.'\nAcompanhe o andamento da mesma na tela Listar Atendimento");
		location.href = "?modulo=principal/listaAtendimento&acao=A";
		</script>');
	
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$menuAba = array(0 => array("id" => 1, "descricao" => "Lista de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaAtendimento&acao=A"),
			  1 => array("id" => 2, "descricao" => "Cadastro de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/cadastraAtendimento&acao=A")
		  	  );

echo montarAbasArray($menuAba, $_SERVER['REQUEST_URI']);

monta_titulo('Cadastro de Atendimento', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');


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
	var tabela = document.getElementById("tabela_mun");
	var tamanho = tabela.rows.length;
		
	if (tamanho < 2){
		alert('O campo "Municípios" é obrigatório.');
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
	selectAllOptions( formulario.solid );

	document.getElementById('submeter').value = '1';
			
	formulario.submit();
	
}






function popupMun(){

	var uf = document.getElementById('uf');
	var listauf = "";

	for(i=0;i<uf.length;i++){
		listauf = listauf + "," + uf[i].value;
	}
	if(listauf) listauf = listauf.substr(1);
	
	w = window.open('atendefnde.php?modulo=principal/popupMun&acao=A&listauf='+listauf,'pesqMun','scrollbars=1,location=0,toolbar=0,menubar=0,status=0,titlebar=0,directories=0,width=370,height=250,left=450,top=125'); 
	w.focus();	
}


function excluiItem(id) {		
	var linha = document.getElementById("linha_"+id).rowIndex;
	var tabela = document.getElementById("tabela_mun");
	tabela.deleteRow(linha);
}

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="submeter" id="submeter" value="" />
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
				//combo_popup( 'uf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', false, false, 4, 400, null, null, $dados_conexao, null, null, true, false, 'filtraMunicipio()', true);
				combo_popup( 'uf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', false, false, 4, 400, null, null, $dados_conexao, null, null, true, false, '', true);
				echo obrigatorio();
	    	?>		
		</td>
	</tr>
	<tr>	
		<td width="33%" align="right" class="SubTituloDireita" valign="top">Municípios:</td>
		<td align="left">

			<div style="margin: 0 0 0 0; padding: 0; float:left; height: 120px; width: 400px; background-color: #eee; border: none;" class="div_rolagem">
				<a href="#" onclick="popupMun();"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir">&nbsp;&nbsp;Inserir Município</a>
				<br>
				<table id="tabela_mun" width="100%" align="left" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<thead>
						<tr id="cabecalho">
							<td width="20%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
							<td width="80%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Município</strong></td>
						</tr>
					</thead>				
					<tbody>
						<?php
							if ($evaid){
							
								$sql = pg_query("
									SELECT  		    
									    to_char(a.agadata::date,'DD/MM/YYYY') as datax,
								            CASE 
								            	WHEN aga_evaid_manha is not null THEN 'Manhã'
								            	WHEN aga_evaid_almoco is not null THEN 'Almoço'
								            	WHEN aga_evaid_tarde is not null THEN 'Tarde'
								            END as turnox,
								            CASE 
								            	WHEN aga_evaid_manha is not null THEN '0' 
								            	WHEN aga_evaid_almoco is not null THEN '1'
								            	WHEN aga_evaid_tarde is not null THEN '2'
								            END || to_char(a.agadata::date,'YYYYMMDD') as id
									FROM  demandas.eventoauditorio e  
									LEFT JOIN demandas.agendaauditorio a ON (e.evaid=a.aga_evaid_manha OR e.evaid=a.aga_evaid_almoco OR e.evaid=a.aga_evaid_tarde)
									WHERE a.agastatus='A' and e.evaid = {$evaid} 
									ORDER BY 1");
		
								$count = 1;
								
								while (($dados = pg_fetch_array($sql)) != false){
									
									$id = $dados["id"];
									$datax = $dados["datax"];
									
									$habilitado = '1';
									if ($habilitado){
										$excluir_modulo = "<img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick=\"excluiItem('". $id ."');\">";
									}else{
										$excluir_modulo = "<img src='/imagens/excluir_01.gif' style='cursor:pointer;' border='0'/>";
									}
									
									$cor = "#f4f4f4";
									$count++;
									if ($count % 2){
										$cor = "#e0e0e0";
									}
									
									echo "
										<tr id=\"linha_".$id."\" bgcolor=\"" . $cor . "\">
											<td align=\"center\">
												" . $excluir_modulo . "
											</td>
											<td align='center'>" . $datax . "<input type='hidden' id='muncod' name='muncod[]' value='" . $datax . "'></td>
										</tr>";
								}
							}
						?>
					</tbody>
				</table>
			</div>
			<div>
			<?echo obrigatorio();?>
			</div>
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
			<input type="button" name="btn_cancelat" value="Cancelar" onclick="location.href='atendefnde.php?modulo=principal/cadastraAtendimento&acao=A';"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>