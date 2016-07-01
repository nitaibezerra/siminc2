<?php

function buscarHistoricoFonteRecurso($dados) {
	global $db;
	
	$sql = "SELECT u.usunome, to_char(h.hfrdata,'dd/mm/YYYY HH24:MI') as data, hfrvalor FROM par.historicofonterecurso h
	 		LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
			WHERE hfrprograma='".$dados['programa']."' AND frpid='".$dados['frpid']."'";
	$cabecalho = array("Usuário","Data","Valor(R$)");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);

}

function gravarFontesRecursos($dados) {
	global $db;
	
	$sql = "UPDATE par.".$dados['programa']."
   			SET frpsaldo=".(($dados['frpsaldo'])?"'".str_replace(array(".",","),array("","."),$dados['frpsaldo'])."'":"NULL")."
 			WHERE frpid='".$dados['frpid']."'";
	
	$db->executar($sql);
	
	$sql = "INSERT INTO par.historicofonterecurso(
            hfrprograma, usucpf, hfrdata, frpid, hfrvalor)
    		VALUES ('".$dados['programa']."', '".$_SESSION['usucpf']."', NOW(), '".$dados['frpid']."', ".(($dados['frpsaldo'])?"'".str_replace(array(".",","),array("","."),$dados['frpsaldo'])."'":"NULL").");";
	
	$db->executar($sql);
	
	$db->commit();
	
	echo "Atualizado com sucesso";
	
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include APPRAIZ."includes/cabecalho.inc";
echo'<br>';

monta_titulo( $titulo_modulo, '' );
?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>

function gravarSaldoFonteRecurso(frpid) {

	var conf = confirm('Deseja realmente atualizar este saldo?');
	
	if(conf) {
		document.getElementById('frpsaldo_'+frpid).value=mascaraglobal('###.###.###,##',document.getElementById('frpsaldo_'+frpid).value);
		var programa = '<?=$_REQUEST['programa'] ?>';
		jQuery.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/gerenciarFontesRecursos&acao=A",
	   		data: "requisicao=gravarFontesRecursos&programa="+programa+"&frpid="+frpid+"&frpsaldo="+document.getElementById('frpsaldo_'+frpid).value,
	   		success: function(msg){
	   			alert(msg);
	   		}
	 		});
 		
 	}

}

function abrirHistorico(obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	
	if(obj.title=='mais') {
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan='6';
		var programa = '<?=$_REQUEST['programa'] ?>';
		
		jQuery.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/gerenciarFontesRecursos&acao=A",
	   		data: "requisicao=buscarHistoricoFonteRecurso&programa="+programa+"&frpid="+obj.id,
	   		success: function(msg){
	   					ncol.innerHTML=msg;
	   		}
	 		});
	 		
		obj.title='menos';
		obj.src='../imagens/menos.gif';
	} else {
		tabela.deleteRow(linha.rowIndex);
		obj.title='mais';
		obj.src='../imagens/mais.gif';
	}
}

function selecionarPrograma(programa) {
	window.location='par.php?modulo=principal/gerenciarFontesRecursos&acao=A&programa='+programa;
}

</script>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTituloDireita">Selecione o programa:</td>
		<td><? $db->monta_combo( "programa", array(0=>array("codigo"=>"fonterecursopac","descricao"=>"PAC"),1=>array("codigo"=>"fonterecursopar","descricao"=>"PAR")), 'S', 'Selecione', 'selecionarPrograma', '', '', '', '', '', '', $_REQUEST['programa'] ); ?></td>
	</tr>
</table>
<?
if($_REQUEST['programa']) {
	
	$_INNER = array("fonterecursopac" => "INNER JOIN par.processoobra pro ON pro.pronumeroprocesso=emp.empnumeroprocesso and pro.prostatus = 'A' ", 
					"fonterecursopar" => "INNER JOIN par.processopar prp ON prp.prpnumeroprocesso=emp.empnumeroprocesso and prp.prpstatus = 'A'");
	
	$sql = "SELECT '<img src=../imagens/mais.gif style=cursor:pointer onclick=abrirHistorico(this); id=\"'||frpid||'\" title=mais>' as acao, 
				   frpcodigo, 
				   '<input type=\"text\" class=\" normal\" title=\"Saldo(R$)\" onblur=\"MouseBlur(this);gravarSaldoFonteRecurso('||frpid||')\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" value=\"'||trim(to_char(frpsaldo,'999g999g999d99'))||'\" maxlength=\"14\" size=\"17\" id=\"frpsaldo_'||frpid||'\" style=\"text-align:;\">' as inp, 
				   frpfuncionalprogramatica, 
				   frptitulo,
				   (SELECT SUM(vve.vrlempenhocancelado) FROM par.empenho emp 
				   	inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid and empstatus = 'A'
				   {$_INNER[$_REQUEST['programa']]}
				   WHERE emp.empfonterecurso=fonte.frpcodigo AND empcodigoespecie not in ('03', '13', '02', '04')) as valor
				   FROM par.".$_REQUEST['programa']." fonte 
				   ORDER BY frpid";
				   
	$cabecalho = array("&nbsp;","CÓDIGO","SALDO(R$)","FUNCIONAL PROGRAMÁTICA","TÍTULO","VALOR(R$)");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%',$par2);
}
?>