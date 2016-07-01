<?php
//AJAX
if($_REQUEST['setor'])
{
	/*if(!$_REQUEST['origem'])
		$_REQUEST['origem'] = 0;*/
	
	//$dtini = ajusta_data($_REQUEST['dataini']);
	//$dtfim = ajusta_data($_REQUEST['datafim']);
	
	
	/*$retorno = lista();
	//ver($retorno,d);die;
	
	$corpoSql = strstr($retorno['sql'],'FROM');
	$corpoSql = str_replace("ORDER BY id DESC"," AND unaid = {$_REQUEST['setor']} ORDER BY id DESC",$corpoSql);
	
	$sql = "SELECT 
					d.dmdid as id,d.dmdtitulo,d.docid,ed.esddsc " . $corpoSql;*/
	
	
	//ver($sql,d);
	/*
	$sql = "SELECT 
			d.dmdid,d.dmdtitulo,d.docid,ed.esddsc
		  FROM 
		  	demandas.demanda AS d
		  	 LEFT JOIN  workflow.documento doc ON doc.docid = d.docid
			 LEFT JOIN  workflow.estadodocumento ed ON ed.esdid = doc.esdid 
			 INNER JOIN demandas.tiposervico as tp on d.tipid = tp.tipid
		  WHERE
			unaid = {$_REQUEST['setor']}
			AND
				tp.ordid in ({$_REQUEST['origem']})
			AND 
				d.dmddatainclusao BETWEEN '$dtini' AND '$dtfim'	
		  ORDER BY 
		  		d.dmdtitulo";*/
	
	$sql = retornaSqlPerfil(" AND unaid = {$_REQUEST['setor']} ", " SELECT d.dmdid as id,d.dmdtitulo,d.docid,ed.esddsc  "); 
	$demandas = $db->carregar($sql);
	
	echo "<table class=\"tabelaVG\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" style='width:100%; margin:0px;'>";
		
		$i = 0;
		
		if($demandas)	
		{
			echo "<tr class='subTitulo'>";
				echo "<td width='8%'>C&oacute;digo</td>";
				echo "<td>Assunto</td>";
				echo "<td width='30%'>Situa&ccedil;&atilde;o</td>";
			echo "</tr>";
			
			foreach($demandas as $demanda)
			{
				$class = ($i%2 == 0) ? 'zebrado1' : 'zebrado2';
				
				echo "<tr class='$class filho1'>";
					echo "<td>{$demanda['id']}</td>";
					echo "<td><a  href='demandas.php?modulo=principal/lista&acao=A&dmdid={$demanda['id']}'> ".($demanda['dmdtitulo'])."</a></td>";
					echo "<td>".($demanda['esddsc'])."</td>";
				echo "</tr>";
				
				$i++;
			}
		}
		else
		{
			echo "<tr>";
				echo "<td colspan='3' align='center' style='color: #f00;'>Nenhum resultado encontrado</td>";
			echo "</tr>";
		}	
	echo "</table>";
	
	exit;	
}

//AJAX
if($_REQUEST['tecnico'])
{
	/*if(!$_REQUEST['origem'])
		$_REQUEST['origem'] = 0;*/
		
	
	
	/*$sql = "SELECT 
			d.dmdid,d.dmdtitulo,d.docid,ed.esddsc
		  FROM 
		  	demandas.demanda AS d
		  	 LEFT JOIN workflow.documento doc ON doc.docid = d.docid
			 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid 
			 INNER JOIN demandas.tiposervico as tp on d.tipid = tp.tipid
		  WHERE
			usucpfexecutor = '{$_REQUEST['tecnico']}'
			AND
				tp.ordid in ({$_REQUEST['origem']})
			AND 
				d.dmddatainclusao BETWEEN '$dtini' AND '$dtfim'	
		  ORDER BY d.dmdtitulo";*/
	
	$dtini = ajusta_data($_REQUEST['dataini']);
	$dtfim = ajusta_data($_REQUEST['datafim']);
	
	$sql = retornaSqlPerfil(" AND usucpfexecutor = '{$_REQUEST['tecnico']}' ORDER BY d.dmdtitulo",  "SELECT d.dmdid,d.dmdtitulo,d.docid,ed.esddsc ");
	
	$demandas = $db->carregar($sql);
	
	echo "<table class=\"tabelaVG\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" style='width:100%; margin:0px;'>";
		
		$i =0;	
		if($demandas)
		{
			echo "<tr class='subTitulo'>";
				echo "<td width='8%'>C&oacute;digo</td>";
				echo "<td>Assunto</td>";
				echo "<td width='30%'>Situa&ccedil;&atilde;o</td>";
			echo "</tr>";
				
			foreach($demandas as $demanda)
			{
				$class = ($i%2 == 0) ? 'zebrado1' : 'zebrado2';
				
				echo "<tr class='$class filho1'>";
					echo "<td>{$demanda['dmdid']}</td>";
					echo "<td><a  href='demandas.php?modulo=principal/lista&acao=A&dmdid={$demanda['dmdid']}'> ".($demanda['dmdtitulo'])."</a></td>";
					echo "<td>".($demanda['esddsc'])."</td>";
				echo "</tr>";
				
				$i++;
			}
		}	
		else
		{
			echo "<tr>";
				echo "<td colspan='3' align='center' style='color: #f00;'>Nenhum resultado encontrado</td>";
			echo "</tr>";
		}	
	echo "</table>";
	exit;
}

//AJAX
if($_REQUEST['corpo'])
{	
	//SE ESTIVER EM BRANCO ADICIONA 0 PARA NÃO QUEBRAR SQL (ordid)
	/*if(!$_REQUEST['origem'])
		$_REQUEST['origem'] = 0;*/
	
	$postdatainicial 	= ($_REQUEST['dataini']);
	$postdatafinal		= ($_REQUEST['datafim']);	
		
	$dtini 			= verifica_data($_REQUEST['dataini']);
	$dtfim 			= verifica_data($_REQUEST['datafim']);
	
	$datainicial 	= ajusta_data($_REQUEST['dataini']);
	$datafinal		= ajusta_data($_REQUEST['datafim']);
	
	if((!$dtini || !$dtfim) || ($datainicial > $datafinal	))
	{
		echo "erro";
	}	
	else
	{	
	?>
		
	<!-- DEMANDAS SEPARADAS POR SETOR -->
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width: 48%">
		<tr class='cabecalhoTabela'>
			<th colspan="3">Demandas do setor</th>
		</tr>
		<tr class='subTitulo'>
			<td>A&ccedil;&atilde;o</td>
			<td>Setor</td>
			<td>Qtde</td>
		</tr>
		<?
		
		
		
			$sql = "SELECT
						unaid, 	
						upper(unasigla)||' - '||unadescricao as descricao 
					FROM
						demandas.unidadeatendimento 
					WHERE
						unastatus = 'A'
					ORDER BY
						unasigla, unadescricao";
			
			$setor 	= $db->carregar($sql);
			
			$i 		= 0;
			$total 	= 0;
			//SETORES COM SUAS DEVIDAS DEMANDAS
			foreach($setor as $unidade)
			{
				/*$sql = " SELECT 
							 count(dem.dmdtitulo) 
						FROM 
							demandas.demanda as dem
						INNER JOIN 
							demandas.tiposervico as tp on dem.tipid = tp.tipid 
						WHERE
							unaid = {$unidade['unaid']} 
						AND
							tp.ordid in ({$_REQUEST['origem']})
						AND 
							dem.dmddatainclusao BETWEEN '$datainicial' AND '$datafinal'";*/
				
				$sql = retornaSqlPerfil(" AND unaid = {$unidade['unaid']}", "SELECT  count(d.dmdid) as id ");
	
				$qtd = $db->pegaUm($sql);
				$total += $qtd;
				
				$class = ($i%2 == 0) ? 'zebrado1' : 'zebrado2';
				$i++;
				
				//DEMANDAS SEPARADAS POR SETOR
				echo"<tr class='$class filho1'>\n";
					echo "<td align='center'><img id='mais_{$unidade['unaid']}' name='mais' src='../imagens/mais.gif' style='cursor: pointer;' onclick='abreconteudo(\"mais_{$unidade['unaid']}\",\"{$unidade['unaid']}\",\"setor\")'></div></td>\n";
					echo "<td>".($unidade['descricao'])."</td>\n";
					echo "<td align='center'>$qtd</td>\n";
				echo"</tr>\n";
				
				//DEMANDAS DE UM DETERMINADO SETOR
				echo "<tr  id='tr_{$unidade['unaid']}' style='display: none;'>";
					echo "<td></td>";
					echo "<td colspan='2' id='td_{$unidade['unaid']}' ></td>";
				echo "</tr>";
			}
		?>
		<tr class='rodapeTabela'>
			<td colspan="2">Total de registro</td>
			<td align ='center' ><?= $total ?></td>
		</tr>
	</table>
	<br>
	<br>
	<div style="width: 48%;">
		<!-- DEMANDAS DO PERÍODO SELECIONADO -->	
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width: 40%; float:left;" >
			<tr class='cabecalhoTabela'>
				<th colspan="2">Vis&atilde;o no Per&iacute;odo Selecionado</th>
			</tr>
			<tr class='subTitulo'>
				<td>Situa&ccedil;&atilde;o</td>
				<td>Qtde</td>
			</tr>
			<?
				//DEMANDAS DO PERÍODO SELECIONADO
				$sqlSit = "SELECT 
								esdid, esddsc
							FROM 
								workflow.estadodocumento
							WHERE 
								esdstatus = 'A'
							AND	
								tpdid in (31)";
				
				$situacoes = $db->carregar($sqlSit);
				//print_r($situacoes);
				
				$i 		= 0;
				$total	= 0;
				foreach($situacoes as $situacao)
				{
					//SELECIONANDO A QUANTIDADE DE CADA TIPO DE
					//DEMANDAS NO PERÍODO SELECIONADO
					/*$sqlDem = " SELECT 
									count(dem.dmdid)
								FROM 
									demandas.demanda as dem
								INNER JOIN workflow.documento as doc ON dem.docid = doc.docid
								INNER JOIN demandas.tiposervico as tp on dem.tipid = tp.tipid
								WHERE
									doc.esdid ={$situacao['esdid']}
								AND
									tp.ordid in ({$_REQUEST['origem']})";
					
					$sqlDem .= " AND dem.dmddatainclusao BETWEEN '$datainicial' AND '$datafinal'";*/				
					
					$esdid = '';

					switch ($situacao['esdid'])
					{
						case "91":  $esdid = "({$situacao['esdid']},107)";break;
						case "92":  $esdid = "({$situacao['esdid']},108)";break;
						case "95":  $esdid = "({$situacao['esdid']},109)";break;
						case "93":  $esdid = "({$situacao['esdid']},111)";break;
						case "94":  $esdid = "({$situacao['esdid']},112)";break;
						case "122": $esdid = "({$situacao['esdid']},123)";break;
						default:	$esdid = "(0)";
					}
					
					
					$sql = retornaSqlPerfil(" AND doc.esdid in {$esdid}", "SELECT  count(d.dmdid) as id ");
					//ver($sql,d);
					$qtd = $db->pegaUm($sql);
					$total += $qtd;
						
					$class = ($i%2 == 0) ? 'zebrado1' : 'zebrado2';
					$i++;
					
					echo"<tr class='$class filho1'>";
						echo "<td>".($situacao['esddsc'])."</td>";
						echo "<td align='center'>$qtd</td>";
					echo"</tr>";
		
				}
				
			?>
			<tr class='rodapeTabela'>
				<td>Total de registro:</td>
				<td align='center'><?=$total ?></td>
			</tr>
		</table>
		<br>
		<br>
		<!-- DEMANDAS DO PERÍODO ATUAL -->
		<table class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align='right' style="width: 40%; margin-top: -27px;">
			<tr class='cabecalhoTabela'>
				<th colspan="2">Visão do Mês Atual</th>
			</tr>
			<tr class='subTitulo'>
				<td>Situa&ccedil;&atilde;o</td>
				<td>Qtde</td>
			</tr>
			<? 		
				$situacoes = $db->carregar($sqlSit);
				
				$i 		= 0;
				$total 	= 0;
				
				$mes = date('m');
				$ano = date('Y');
				$dia = ultimoDia($mes);
				
				foreach($situacoes as $situacao)
				{
					//QUANTIDADE DE DEMANDAS ATUAIS
					/*$sqlDem = " SELECT 
								count(dem.dmdid)
							FROM 
								demandas.demanda as dem
							INNER JOIN workflow.documento as doc ON dem.docid = doc.docid
							INNER JOIN demandas.tiposervico as tp on dem.tipid = tp.tipid
							WHERE
								doc.esdid ={$situacao['esdid']}
							AND
								tp.ordid in ({$_REQUEST['origem']})";
					
					$sqlDem .= " AND dem.dmddatainclusao BETWEEN '$ano-$mes-01' AND '$ano-$mes-$dia'";*/
					
					$_POST["dataini"] = date( "01/m/Y" );
					$_POST["datafim"] = ultimoDia( date( "m" ) ).date( "/m/Y" );
					
					$sql = retornaSqlPerfil(" AND doc.esdid in {$esdid}", "SELECT  count(d.dmdid) as id ");
	
					$qtd 	= $db->pegaUm($sql);
					$total 	+= $qtd;
					
					$class = ($i%2 == 0) ? 'zebrado1' : 'zebrado2';
					$i++;
					
					echo"<tr class='$class filho1'>";
						echo "<td>".($situacao['esddsc'])."</td>";
						echo "<td align='center'>$qtd</td>";
					echo"</tr>";
				}
			?>	
			<tr class='rodapeTabela'>
				<td>Total de registro:</td>
				<td align="center"><?=$total ?></td>
			</tr>
		</table>
	</div>	
	
	<!-- PONTUAÇÃO POR TÉCNICO -->
	<table class="listagem" cellSpacing="1" cellPadding="3" style="margin-top: 2%; width: 48%;">
		<tr class='cabecalhoTabela'>
			<th colspan="3">Pontua&ccedil;&atilde;o por T&eacute;cnico</th>
		</tr>
		<tr class='subTitulo'>
			<td>A&ccedil;&atilde;o</td>
			<td>T&eacute;cnico</td>
			<td>Pontua&ccedil;&atilde;o</td>
		</tr>
		<?php
			//LISTA DOS TÉCNICOS
			/*$sql = "SELECT 
						distinct(usucpfexecutor),usunome
					FROM 
						demandas.demanda as dem
					INNER JOIN seguranca.usuario as usu ON usu.usucpf = dem.usucpfexecutor
					INNER JOIN demandas.tiposervico as tp on dem.tipid = tp.tipid
					WHERE
						tp.ordid in ({$_REQUEST['origem']})
					ORDER BY 
						usunome";*/

			$_POST["dataini"] = $postdatainicial;
			$_POST["datafim"] = $postdatafinal;
								
			
			$sql = retornaSqlPerfil(" ", "SELECT distinct(d.usucpfexecutor) ");
			//ver($sql);
			/*$sqlWhere = strstr($sql,'WHERE');
			
			$sql = "SELECT 
						distinct(usucpfexecutor),usunome
					FROM	 
						demandas.demanda as d
					INNER JOIN seguranca.usuario as usu ON usu.usucpf = d.usucpfexecutor
					INNER JOIN demandas.tiposervico as tp on d.tipid = tp.tipid 
					INNER JOIN workflow.documento doc ON doc.docid = d.docid 
					LEFT JOIN seguranca.usuario ur2 ON ur2.usucpf = d.usucpfexecutor " . $sqlWhere;*/
			
			
			
			$cpfTecnicos = $db->carregar($sql);
			if($cpfTecnicos)
			{
				$arrayCPF = array();
				
				foreach($cpfTecnicos as $cpfTecnico)
				{
					if($cpfTecnico['usucpfexecutor'])
						$arrayCPF[] = " '{$cpfTecnico['usucpfexecutor']}'";
				}
				$arrayCPF = implode(',',$arrayCPF);
			}		
		
			if($arrayCPF)
			{
				$sql = "SELECT usucpf,usunome FROM seguranca.usuario WHERE usucpf in($arrayCPF) ORDER BY usunome";
							
				$tecnicos = $db->carregar($sql);
				
				$i 		= 0;
				$total 	= 0;
								
				//LISTA DE TÉCNICOS
				if($tecnicos)
				{
					foreach($tecnicos as $tecnico)
					{
						if($tecnico['usucpf'])
						{
							$class = ($i%2 == 0) ? 'zebrado1' : 'zebrado2';
							$i++;
							
							//$nomeTecnico = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf = '{$tecnico['usucpf']}'");
							
							//QUANTIDADE DE PONTOS DE CADA TÉCNICO
							/*$sqlQtDem = "SELECT 
											sum(tsp.tsppontuacao)
										FROM 
											demandas.demanda as dem
										
										INNER JOIN demandas.tiposervico AS ts ON dem.tipid = ts.tipid
										INNER JOIN demandas.tiposervicoprioridade AS tsp ON ts.tipid = tsp.tipid
										INNER JOIN demandas.tiposervico as tp on dem.tipid = tp.tipid
										WHERE 
											dem.usucpfexecutor = '{$tecnico['usucpfexecutor']}'
										AND
											tp.ordid in ({$_REQUEST['origem']})
										AND 
											dem.dmddatainclusao BETWEEN '$datainicial' AND '$datafinal'		";*/
							
							$sql = retornaSqlPerfil(" AND d.usucpfexecutor = '{$tecnico['usucpf']}' ", "SELECT sum(tsp.tsppontuacao) ");
		
							$sql = str_replace('WHERE', "INNER JOIN demandas.tiposervicoprioridade AS tsp ON t.tipid = tsp.tipid WHERE ", $sql);
							//ver($sql,d);				
							$qtd 	= $db->pegaUm($sql);
							if($qtd == '')
								$qtd = 0;
								
							$total 	+= $qtd;
							
							echo "<tr class='$class filho1'>\n";
								echo "<td align='center'><img id='mais_{$tecnico['usucpf']}' name='mais' src='../imagens/mais.gif' style='cursor: pointer;' onclick='abreconteudo(\"mais_{$tecnico['usucpf']}\",\"{$tecnico['usucpf']}\",\"tecnico\")'></div></td>\n";
								echo "<td>".($tecnico['usunome'])."</td>\n";
								echo "<td align='center'>$qtd</td>\n";
							echo "</tr>\n";	
							
							echo "<tr  id='tr_".($tecnico['usucpf'])."' style='display: none;'>";
								echo "<td></td>";
								echo "<td colspan='2' id='td_{$tecnico['usucpf']}' ></td>";
							echo "</tr>";
						}	
					}
				}
				else
				{
					echo "<tr>";
						echo "<td colspan='3' align='center' style='color: #f00;'>Nenhum resultado encontrado</td>";
					echo "</tr>";	
				}
			}
			else
			{
				echo "<tr>";
					echo "<td colspan='3' align='center' style='color: #f00;'>Nenhum resultado encontrado</td>";
				echo "</tr>";	
			}	
		
		?>
		<tr class='rodapeTabela'>
			<td colspan="2">Total de registro:</td>
			<td align="center"><?=$total ?></td>
		</tr>
	</table>
<?php	
	}
exit;
}
// FIM CORPO

function retornaSqlPerfil($params,$select)
{	
	$retorno = lista();
		
	$corpoSql = strstr($retorno['sql'],'FROM');
	
	$corpoSql = str_replace("ORDER BY id DESC",$params,$corpoSql);
	
	$sql = $select . $corpoSql;
	
	return $sql;
}

function ultimoDia($mes){
	return date('d',strtotime('-1 second',strtotime('+1 month',strtotime($mes.'/01/'.date('Y').' 00:00:00'))));
}	

include  APPRAIZ . 'includes/cabecalho.inc';



global $db;
/*
$sql = "SELECT 
			distinct(ordid), celid
		FROM 
			demandas.usuarioresponsabilidade

		WHERE 
			rpustatus = 'A' 
		AND	
			usucpf = '{$_SESSION['usucpf']}'";

$origemD = $db->carregar($sql);

$GLOBALS['textoOrigem'] = array();

if($origemD)
{
	foreach($origemD as $origem)
	{
		
		if($origem && !in_array($origem,$GLOBALS['textoOrigem']))
		{
			if($origem['ordid'])
				$GLOBALS['textoOrigem'][] = $origem['ordid'];	
			
			if($origem['celid'])
			{
				$sql = "SELECT 
						ordid
					FROM 
						demandas.celula
					WHERE
						celstatus = 'A' 
					AND		
						celid = {$origem['celid']}";
						
				$GLOBALS['textoOrigem'][] = $db->pegaUm($sql);
						
			}			
		}	
	}	
}
	
$GLOBALS['textoOrigem'] = implode(',',$GLOBALS['textoOrigem']);*/

?>
<script type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

function abreconteudo(img,id,params)
{
	imagem = document.getElementById(img);

	td = document.getElementById('td_' + id);
	tr = document.getElementById('tr_' + id);
	
	if(imagem.name == 'mais')
	{
		imagem.name = 'menos';
		imagem.src='../imagens/menos.gif';
		
		var dtini = document.getElementById('dataini').value;
		var dtfim = document.getElementById('datafim').value;
		
		string = '&dataini=' + dtini + '&datafim=' + dtfim ;

		
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/visaoGerencial&acao=A', {
				        method:     'post',
				        parameters: '&' + params + '=' + id + string,
				        onComplete: function (res)
				        {	
							tr.style.display = '';
							td.innerHTML = res.responseText;
				        }
				  });
	}
	else
	{
		imagem.name = 'mais';
		imagem.src= '../imagens/mais.gif';
		tr.style.display = 'none';
		td.innerHTML = '';
	}
}

function validaForm(form)
{
	var string = '';
	for(var i = 0; i< form.length; i++)
	{
		if(form[i].type == 'text')
		{
			if(form[i].value == '')
			{
				alert('Os campos datas são obrigatórios.');
				form[i].focus();
				return;		
			}
		}
	}
	string += '&dataini=' + form[0].value + '&datafim=' + form[1].value;
	
	document.getElementById('bloqueio').style.display = 'inline-block';
	
	var req = new Ajax.Request('demandas.php?modulo=principal/visaoGerencial&acao=A', {
				        method:     'post',
				        parameters: '&corpo=true' + string,
				        onComplete: function (res)
				        {	
				        	if(res.responseText == 'erro')
				        		alert('Datas inválidas');
				        	else
				        		document.getElementById('content').innerHTML = res.responseText;
				        		
				        	document.getElementById('bloqueio').style.display = 'none';	
				        }
				  });

}

function teclaEnter(event,form)
{
	if(event == 13)
		validaForm(form);
}			  
</script>

<style>
	.subTitulo{
		border-bottom-color: #f00;
	}
	.subTitulo td{
		text-align: center;
		background-color: #E9E9E9;
		font-weight: bold;
		border: 1px solid #acacac;
	}

	.rodapeTabela{
		background-color: #fff; 
	}	
	
	.cabecalhoTabela{
		background-color: #fff;
	}
	
	.cabecalhoTabela th{
		border: 1px solid #aeaeae;
		background-color: #F5F5F5;
	}
	.rodapeTabela td{
		font-weight: bold;
		border: 1px solid #cecece;
	}
	.tabelaVG{
		border: 1px solid #cecece;
		border-collapse: collapse;
		background-color: transparent;
		width: 48%;
		
		margin-left: 2.5%;
		margin-bottom: 10px;
	}
	.tabelaVG a {
		color:#00F;
		text-decoration: underline;
	}
	.zebrado1{
		background-color: #fff;
	}
	.zebrado2{
		background-color: #F5F5F5;
	}
	
	tr.filho1:hover{
		background: #ffffcc;
	}		
	#content{
		width: 1200px;
		margin-left: 2.5%;
	}
	
	#corpo{
		width: 1200px;
	}
	#bloqueio{
		background-color:#ffffff;
		position:absolute;
		color:#000033;
		top:50%;
		left:30%;
		border:2px solid #cccccc; 
		width:300px;
		display: none;
	}
</style>
<div id='corpo'>
<br>
<?php
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo('Origem da demanda','');
?>
<div id="bloqueio">
	<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Carregando dados...<br><br></center>
</div>
	<!--Cabeçalho  -->
	<form id='formulario' name='formulario' action="demandas.php?modulo=principal/visaoGerencial&acao=A">
		<table class='tabela' bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td  class="SubTituloDireita">Período:</td>
				<td>
					<?=campo_data('dataini', 'N', 'S', '', 'S');?>
					&nbsp;&nbsp;&nbsp;&nbsp;
					até
					&nbsp;&nbsp;&nbsp;&nbsp;
					<?=campo_data('datafim', 'N', 'S', '', 'S');?>
				</td>	
			</tr>
			<tr>
				<td></td>
				<td></td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td></td>
				<td><input type="button" value="Pesquisar" onclick="validaForm(this.form)";> </td>
			</tr>
		</table>
	</form>	
	<br>
	<br>
	<div id='content' ></div>
</div>	
