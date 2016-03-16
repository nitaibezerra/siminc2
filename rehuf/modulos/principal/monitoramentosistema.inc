<?

function tmpmedproc($ano, $mes, $sisid) {
	global $db;
	$sql = "SELECT ROUND(CAST(AVG(esttempoexec)as numeric),2) as num, to_char(estdata, 'dd') as dia FROM seguranca.estatistica WHERE sisid='".$sisid."' AND to_char(estdata, 'YYYY-mm')='".$ano."-".$mes."' GROUP BY to_char(estdata, 'dd') ORDER BY dia";
	$dados = $db->carregar($sql);
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc[$d['dia']] = $d['num'];
		}
	}
	return $dadosc;
	
}
function numerr($ano, $mes, $sisid) {
	global $db;
	$sql = "SELECT COUNT(au.oid) as num, to_char(auddata, 'dd') as dia FROM seguranca.auditoria au 
		    INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
			WHERE me.sisid='".$sisid."' AND au.audtipo='X' AND to_char(auddata, 'YYYY-mm')='".$ano."-".$mes."' 
			GROUP BY to_char(auddata, 'dd') ORDER BY dia";
	
	$dados = $db->carregar($sql);
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc[$d['dia']] = $d['num'];
		}
	}
	return $dadosc;
}
function numreq($ano, $mes, $sisid) {
	global $db;
	$sql = "SELECT COUNT(oid) as num, to_char(estdata, 'dd') as dia FROM seguranca.estatistica WHERE sisid='".$sisid."' AND to_char(estdata, 'YYYY-mm')='".$ano."-".$mes."' GROUP BY to_char(estdata, 'dd') ORDER BY dia";
	$dados = $db->carregar($sql);
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc[$d['dia']] = $d['num'];
		}
	}
	return $dadosc;
	
}
function numusudis($ano, $mes, $sisid) {
	global $db;
	$sql = "SELECT COUNT(DISTINCT usucpf) as num, to_char(estdata, 'dd') as dia FROM seguranca.estatistica WHERE sisid='".$sisid."' AND to_char(estdata, 'YYYY-mm')='".$ano."-".$mes."' GROUP BY to_char(estdata, 'dd') ORDER BY dia";
	$dados = $db->carregar($sql);
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc[$d['dia']] = $d['num'];
		}
	}
	return $dadosc;
		
}

function numupd($ano, $mes, $sisid) {
	global $db;
	$sql = "SELECT COUNT(au.oid) as num, to_char(auddata, 'dd') as dia FROM seguranca.auditoria au 
			INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
			WHERE me.sisid='".$sisid."' AND to_char(auddata, 'YYYY-mm')='".$ano."-".$mes."' AND audtipo='U'
			GROUP BY to_char(auddata, 'dd') ORDER BY dia";
	
	$dados = $db->carregar($sql);
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc[$d['dia']] = $d['num'];
		}
	}
	return $dadosc;
	
}

function numins($ano, $mes, $sisid) {
	global $db;
	$sql = "SELECT COUNT(au.oid) as num, to_char(auddata, 'dd') as dia FROM seguranca.auditoria au 
			INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
			WHERE me.sisid='".$sisid."' AND to_char(auddata, 'YYYY-mm')='".$ano."-".$mes."' AND audtipo='I'
			GROUP BY to_char(auddata, 'dd') ORDER BY dia";
	
	$dados = $db->carregar($sql);
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc[$d['dia']] = $d['num'];
		}
	}
	return $dadosc;
}

function numdel($ano, $mes, $sisid) {
	global $db;
	$sql = "SELECT COUNT(au.oid) as num, to_char(auddata, 'dd') as dia FROM seguranca.auditoria au 
			INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
			WHERE me.sisid='".$sisid."' AND to_char(auddata, 'YYYY-mm')='".$ano."-".$mes."' AND audtipo='I'
			GROUP BY to_char(auddata, 'dd') ORDER BY dia";
	
	$dados = $db->carregar($sql);
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc[$d['dia']] = $d['num'];
		}
	}
	return $dadosc;
}

function relerrreq($ano, $mes, $sisid) {
	global $db;
	$sql = "SELECT COUNT(au.oid) as num, to_char(auddata, 'dd') as dia FROM seguranca.auditoria au 
		    INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
			WHERE me.sisid='".$sisid."' AND au.audtipo='X' AND to_char(auddata, 'YYYY-mm')='".$ano."-".$mes."' 
			GROUP BY to_char(auddata, 'dd') ORDER BY dia";
	$dados = $db->carregar($sql);
	
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc1[$d['dia']] = $d['num'];
		}
	}
	
	$sql = "SELECT COUNT(oid) as num, to_char(estdata, 'dd') as dia FROM seguranca.estatistica WHERE sisid='".$sisid."' AND to_char(estdata, 'YYYY-mm')='".$ano."-".$mes."' GROUP BY to_char(estdata, 'dd') ORDER BY dia";
	$dados = $db->carregar($sql);
	
	if($dados[0]) {
		foreach($dados as $d) {
			$dadosc2[$d['dia']] = $d['num'];
		}
	}
	
	if($dadosc1) {
		foreach($dadosc1 as $dia => $d) {
			$dadosc[$dia] = (($dadosc2[$dia])?round($d/$dadosc2[$dia],4):"");
		}
	}
	
	return $dadosc;
	
}


function carregarGRIDMonitoramento($dados) {
	global $db;
	
	$infos = array("tmpmedproc" => "Tempo Médio de Processamento por página (Segundos)",
			       "numerr" => "Número de erros",
			   	   "numreq" =>"Número de requisições",
			       "numusudis" => "Número de Usuários Distintos",
			       "numupd" => "Número de UPDATEs",
			       "numins" => "Número de INSERTs",
			       "numdel" => "Número de DELETEs");
	
	
	set_time_limit(360);
	
	// carregando o número de dias do mês	
	$ndias = cal_days_in_month(CAL_GREGORIAN, $dados['mes'], $dados['ano']);
	
	$_HTML = "<table class=\"listagem\" width=\"95%\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
		
	$_HTML .= "<tr>";
	$_HTML .= "<td class=\"SubTituloCentro\">Informações</td>";
	
	for($i=1;$i<=$ndias;$i++) {
		
		if(date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 0 || 
	   	   date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 6) {
	   	   	
			$_HTML .= "<td class=\"SubTituloCentro\" style=\"background-color:#FFFFCC;width:25px;\">".$i."</td>";
	   	   	
	   	} else {
		
			$_HTML .= "<td class=\"SubTituloCentro\" style=\"width:25px;\">".$i."</td>";
			
	   	}
				

	}
	$_HTML .= "</tr>";
	
		
	$dadostr = $dados['info']($dados['ano'],$dados['mes'], $dados['sisid']);
	
	$_HTML .= "<tr>";
	$_HTML .= "<td class=\"SubTituloDireita\">".$infos[$dados['info']]."</td>";
	
	for($i=1;$i<=$ndias;$i++) {
		
		if(date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 0 || 
	   	   date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 6) {
	   	   	
			$_HTML .= "<td style=\"text-align:right;background-color:#FFFFCC;width:25px;\">".(($dadostr[sprintf("%02d", $i)])?$dadostr[sprintf("%02d", $i)]:"-")."</td>";
	   	   	
	   	} else {
		
			$_HTML .= "<td style=\"text-align:right;width:25px;\">".(($dadostr[sprintf("%02d", $i)])?$dadostr[sprintf("%02d", $i)]:"-")."</td>";
			
	   	}

	}
	$_HTML .= "</tr>";
		
	$_HTML .= "</table>";
	
	echo $_HTML;
	
}
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Monitoramento dos módulos do SIMEC", "");


?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script>
function ajaxatualizar(params,iddestinatario, pai) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}else{
						document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}	
				} 
			},
			onLoading: function(){
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}else{
						document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}
				}	
			}
		});
}

function atualizarGridMonitoramento(valor) {
	if(document.getElementById('mes').value &&
	   document.getElementById('ano').value && 
	   document.getElementById('info').value &&
	   document.getElementById('sisid').value) {
	   
		document.getElementById('gridMonitoramento').innerHTML='<p align="center"><img src="/imagens/carregando.gif" align="absmiddle"> Carregando...</p>';
	   
		ajaxatualizar('requisicao=carregarGRIDMonitoramento&mes='+document.getElementById('mes').value+'&ano='+document.getElementById('ano').value+'&sisid='+document.getElementById('sisid').value+'&info='+document.getElementById('info').value, 'gridMonitoramento');
		
	} else {
		document.getElementById('gridMonitoramento').innerHTML="";	
	}
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'>Mês :</td>
	<td>
	<?
	$mes=date("m");
	$sql = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
	$db->monta_combo('mes', $sql, 'S', 'Selecione', 'atualizarGridMonitoramento', '', '', '', 'S', 'mes');
	?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Ano :</td>
	<td>
	<?
	$ano=date("Y");
	$sql = "SELECT ano as codigo, ano as descricao FROM public.anos";
	$db->monta_combo('ano', $sql, 'S', 'Selecione', 'atualizarGridMonitoramento', '', '', '', 'S', 'ano');
	?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Informação :</td>
	<td>
	<?
	$infos = array(0 => array("codigo" => "tmpmedproc", "descricao" => "Tempo Médio de Processamento por página (Segundos)"),
				   1 => array("codigo" => "numerr", "descricao" => "Número de erros"),
				   2 => array("codigo" => "numreq", "descricao" => "Número de requisições"),
				   3 => array("codigo" => "numusudis", "descricao" => "Número de Usuários Distintos"),
				   4 => array("codigo" => "numupd", "descricao" => "Número de UPDATEs"),
				   5 => array("codigo" => "numins", "descricao" => "Número de INSERTs"),
				   6 => array("codigo" => "numdel", "descricao" => "Número de DELETEs")
				   );
	
	$db->monta_combo('info', $infos, 'S', 'Selecione', 'atualizarGridMonitoramento', '', '', '', 'S', 'info');
	?>
	</td>
</tr>


<tr>
	<td class='SubTituloDireita'>Sistema :</td>
	<td>
	<?
	$sisid=$_SESSION['sisid'];
	$sql = "SELECT sisid as codigo, sisdsc as descricao FROM seguranca.sistema";
	$db->monta_combo('sisid', $sql, 'S', 'Selecione', 'atualizarGridMonitoramento', '', '', '', 'S', 'sisid');
	?>
	</td>
</tr>
</table>
<div id="gridMonitoramento"></div>