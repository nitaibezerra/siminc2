<?

// Recupera o perfil/responsabilidade do usuário no Mais Educação.
$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A'";
$pflcod = $db->carregar($sql);

if($pflcod[0]["pflcod"] == PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO) {
	$sql = "SELECT entid FROM pdeescola.meloteimpressao WHERE muncod = '".$pflcod[0]["muncod"]."'";
	$entids = $db->carregar($sql);
	
	if($entids) {
		$restricaoLote = array();
		for($i=0; $i<count($entids); $i++) {
			$restricaoLote[$i] = $entids[$i]["entid"];
		}
		$restricaoLote = " AND e.entid not in (".implode(',', $restricaoLote).")";
	} else {
		$restricaoLote = "";
	}
	
	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	to_char(max(htddata), 'DD/MM/YYYY') as data,
			 	est.esddsc
			FROM
				entidade.entidade e
			INNER JOIN 
				entidade.endereco ee ON e.entid = ee.entid
			INNER JOIN 
				entidade.endereco endi ON endi.endid = ee.endid
			INNER JOIN 
				territorios.municipio m ON m.muncod = endi.muncod
			INNER JOIN 
				pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
											  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . " 
											  AND maedu.memstatus = 'A'
			LEFT JOIN 
				workflow.documento d ON d.docid = maedu.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			LEFT JOIN
				workflow.historicodocumento htd ON htd.docid = d.docid
			WHERE
				m.muncod = '".$pflcod[0]["muncod"]."' AND 
				e.tpcid IN (3) AND
				UPPER(trim(est.esddsc)) = 'FINALIZADO' 
				".$restricaoLote."
			GROUP BY
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	est.esddsc,
			 	maedu.memid";
}
else if($pflcod[0]["pflcod"] == PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO) {
	$sql = "SELECT entid FROM pdeescola.meloteimpressao WHERE estuf = '".$pflcod[0]["estuf"]."'";
	$entids = $db->carregar($sql);
	
	if($entids) {
		$restricaoLote = array();
		for($i=0; $i<count($entids); $i++) {
			$restricaoLote[$i] = $entids[$i]["entid"];
		}
		$restricaoLote = " AND e.entid not in (".implode(',', $restricaoLote).")";
	} else {
		$restricaoLote = "";
	}
	
	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	to_char(max(htddata), 'DD/MM/YYYY') as data,
			 	est.esddsc
			FROM
				entidade.entidade e
			INNER JOIN 
				entidade.endereco ee ON e.entid = ee.entid
			INNER JOIN 
				entidade.endereco endi ON endi.endid = ee.endid
			INNER JOIN 
				territorios.municipio m ON m.muncod = endi.muncod
			INNER JOIN 
				pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
											  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . "
											  AND maedu.memstatus = 'A'
			LEFT JOIN 
				workflow.documento d ON d.docid = maedu.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			LEFT JOIN
				workflow.historicodocumento htd ON htd.docid = d.docid
			WHERE
				m.estuf = '".$pflcod[0]["estuf"]."' AND
				e.tpcid IN (1) AND
				UPPER(trim(est.esddsc)) = 'FINALIZADO'
				".$restricaoLote."
			GROUP BY
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	est.esddsc,
			 	maedu.memid";
}
else {
	echo "<script>
			alert('Não há nenhum Munícipio ou Estado associoado ao seu perfil.');
			window.close();
		  </script>";
	exit;
}

$entidades = $db->carregar($sql);

if($entidades) {
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formListaEscolas" method="post" action="pdeescola.php?modulo=merelatorio/plano_atendimento_geral_consolidado&acao=A">
<input type="hidden" id="lista_escolas" name="lista_escolas" value="" />
<center><b>Informe as escolas a serem visualizadas:</b></center>
<table id="tabela_lista_escolas" width="100%" align="center" border="0" cellspacing="2" cellpadding="5" class="listagem">
<thead>
	<tr>
		<td width="10%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Cod</strong></td>
		<td width="50%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
		<td width="10%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>
		<td width="20%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Situação</strong></td>
		<td width="10%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Incluir</strong></td>
	</tr>
</thead>				
<tbody>
<?
	for($i=0; $i<count($entidades); $i++) {
		$bgColor = ($i % 2) ? "#E8E8E8" : "#FAFAFA";
								
		echo "<tr style=\"background-color:".$bgColor.";\">
				<td valign=\"middle\" align=\"center\">
					".$entidades[$i]["entcodent"]."
					<input type=\"hidden\" name=\"entid[]\" value=\"".$entidades[$i]["entid"]."\" />
				</td>
				<td valign=\"middle\" align=\"center\">".$entidades[$i]["entnome"]."</td>
				<td valign=\"middle\" align=\"center\">".(($entidades[$i]["data"]) ? $entidades[$i]["data"] : "-")."</td>
				<td valign=\"middle\" align=\"center\">".(($entidades[$i]["esddsc"]) ? $entidades[$i]["esddsc"] : "Não Iniciado")."</td>
				<td valign=\"middle\" align=\"center\">
					<input name=\"incluir[]\" type=\"checkbox\" />
				</td>
			  </tr>";
	}
?>
</tbody>
<tfoot>
	<tr style="background-color: #c0c0c0;">
		<td colspan="5" align="right">
			<input type="button" id="bt_cancelar" style="width:100px; cursor:pointer;" onclick="window.close();" value="Cancelar" />
			<input type="button" id="bt_ok" style="width:100px; cursor:pointer;" onclick="submeteListaEscolas();" value="OK" />
		</td>
	</tr>
</tfoot>
</table>
</form>
<script>

function submeteListaEscolas() {
	var listaEscolas 	= document.getElementById("lista_escolas");
	var form			= document.getElementById("formListaEscolas");
	var btCancelar		= document.getElementById("bt_cancelar");
	var btOk			= document.getElementById("bt_ok");
	var checkIncluir 	= document.getElementsByName("incluir[]");
	var entid			= document.getElementsByName("entid[]");
	
	btCancelar.disabled = true;
	btOk.disabled 		= true;
	
	listaEscolas.value 	= "";
	var arrEscolas		= new Array();
	var contador 		= 0;
	
	for(var i=0; i<checkIncluir.length; i++) {
		if(checkIncluir[i].checked == true) {
			arrEscolas.push(entid[i].value);
			contador++;
		}
	}
	
	if(contador == 0) {
		alert("Pelo menos uma escola deve ser selecionada.");
		btCancelar.disabled = false;
		btOk.disabled 		= false;
		return;
	} else {
		listaEscolas.value = arrEscolas.join(",");
		form.submit();
	}
}
	
</script>
<?

} else {
	echo "<script>
			alert('Não existem entidades finalizadas no momento e/ou já foram gerados relatórios para\\ntodas as entidades associadas. Utilize a lista abaixo para rever algum relatório.');
			window.close();
		  </script>";
	exit;
}

?>