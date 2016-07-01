<?php

include_once "./_funcoesagendamentoscripts.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo("Agendamento scripts", "Agendamento para execução dos scripts");

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/seguranca.js"></script>
<script language="javascript" type="text/javascript" src="../includes/open_flash_chart/swfobject.js"></script>
<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td>
	<p><b>Diário</b></p>
	<?
	$sql = "SELECT '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=00\">'||SUM(d0)||'<br>'||TO_CHAR((SUM(d0t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d0,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=01\">'||SUM(d1)||'<br>'||TO_CHAR((SUM(d1t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d1,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=02\">'||SUM(d2)||'<br>'||TO_CHAR((SUM(d2t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d2,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=03\">'||SUM(d3)||'<br>'||TO_CHAR((SUM(d3t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d3,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=04\">'||SUM(d4)||'<br>'||TO_CHAR((SUM(d4t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d4,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=05\">'||SUM(d5)||'<br>'||TO_CHAR((SUM(d5t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d5,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=06\">'||SUM(d6)||'<br>'||TO_CHAR((SUM(d6t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d6,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=07\">'||SUM(d7)||'<br>'||TO_CHAR((SUM(d7t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d7,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=08\">'||SUM(d8)||'<br>'||TO_CHAR((SUM(d8t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d8,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=09\">'||SUM(d9)||'<br>'||TO_CHAR((SUM(d9t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d9,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=10\">'||SUM(d10)||'<br>'||TO_CHAR((SUM(d10t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d10,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=11\">'||SUM(d11)||'<br>'||TO_CHAR((SUM(d11t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d11,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=12\">'||SUM(d12)||'<br>'||TO_CHAR((SUM(d12t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d12,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=13\">'||SUM(d13)||'<br>'||TO_CHAR((SUM(d13t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d13,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=14\">'||SUM(d14)||'<br>'||TO_CHAR((SUM(d14t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d14,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=15\">'||SUM(d15)||'<br>'||TO_CHAR((SUM(d15t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d15,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=16\">'||SUM(d16)||'<br>'||TO_CHAR((SUM(d16t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d16,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=17\">'||SUM(d17)||'<br>'||TO_CHAR((SUM(d17t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d17,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=18\">'||SUM(d18)||'<br>'||TO_CHAR((SUM(d18t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d18,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=19\">'||SUM(d19)||'<br>'||TO_CHAR((SUM(d19t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d19,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=20\">'||SUM(d20)||'<br>'||TO_CHAR((SUM(d20t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d20,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=21\">'||SUM(d21)||'<br>'||TO_CHAR((SUM(d21t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d21,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=22\">'||SUM(d22)||'<br>'||TO_CHAR((SUM(d22t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d22,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=diario&agsperdetalhes=23\">'||SUM(d23)||'<br>'||TO_CHAR((SUM(d23t)||' second')::interval, 'HH24:MI:SS')||'</a>' as d23 
			FROM (
			SELECT
			CASE WHEN agsperdetalhes ilike '%00%' THEN 1 ELSE 0 END as d0,
			CASE WHEN agsperdetalhes ilike '%00%' THEN agstempoexecucao ELSE 0 END as d0t,
			CASE WHEN agsperdetalhes ilike '%01%' THEN 1 ELSE 0 END as d1,
			CASE WHEN agsperdetalhes ilike '%01%' THEN agstempoexecucao ELSE 0 END as d1t,
			CASE WHEN agsperdetalhes ilike '%02%' THEN 1 ELSE 0 END as d2,
			CASE WHEN agsperdetalhes ilike '%02%' THEN agstempoexecucao ELSE 0 END as d2t,
			CASE WHEN agsperdetalhes ilike '%03%' THEN 1 ELSE 0 END as d3,
			CASE WHEN agsperdetalhes ilike '%03%' THEN agstempoexecucao ELSE 0 END as d3t,
			CASE WHEN agsperdetalhes ilike '%04%' THEN 1 ELSE 0 END as d4,
			CASE WHEN agsperdetalhes ilike '%04%' THEN agstempoexecucao ELSE 0 END as d4t,
			CASE WHEN agsperdetalhes ilike '%05%' THEN 1 ELSE 0 END as d5,
			CASE WHEN agsperdetalhes ilike '%05%' THEN agstempoexecucao ELSE 0 END as d5t,
			CASE WHEN agsperdetalhes ilike '%06%' THEN 1 ELSE 0 END as d6,
			CASE WHEN agsperdetalhes ilike '%06%' THEN agstempoexecucao ELSE 0 END as d6t,
			CASE WHEN agsperdetalhes ilike '%07%' THEN 1 ELSE 0 END as d7,
			CASE WHEN agsperdetalhes ilike '%07%' THEN agstempoexecucao ELSE 0 END as d7t,
			CASE WHEN agsperdetalhes ilike '%08%' THEN 1 ELSE 0 END as d8,
			CASE WHEN agsperdetalhes ilike '%08%' THEN agstempoexecucao ELSE 0 END as d8t,
			CASE WHEN agsperdetalhes ilike '%09%' THEN 1 ELSE 0 END as d9,
			CASE WHEN agsperdetalhes ilike '%09%' THEN agstempoexecucao ELSE 0 END as d9t,
			CASE WHEN agsperdetalhes ilike '%10%' THEN 1 ELSE 0 END as d10,
			CASE WHEN agsperdetalhes ilike '%10%' THEN agstempoexecucao ELSE 0 END as d10t,
			CASE WHEN agsperdetalhes ilike '%11%' THEN 1 ELSE 0 END as d11,
			CASE WHEN agsperdetalhes ilike '%11%' THEN agstempoexecucao ELSE 0 END as d11t,
			CASE WHEN agsperdetalhes ilike '%12%' THEN 1 ELSE 0 END as d12,
			CASE WHEN agsperdetalhes ilike '%12%' THEN agstempoexecucao ELSE 0 END as d12t,
			CASE WHEN agsperdetalhes ilike '%13%' THEN 1 ELSE 0 END as d13,
			CASE WHEN agsperdetalhes ilike '%13%' THEN agstempoexecucao ELSE 0 END as d13t,
			CASE WHEN agsperdetalhes ilike '%14%' THEN 1 ELSE 0 END as d14,
			CASE WHEN agsperdetalhes ilike '%14%' THEN agstempoexecucao ELSE 0 END as d14t,
			CASE WHEN agsperdetalhes ilike '%15%' THEN 1 ELSE 0 END as d15,
			CASE WHEN agsperdetalhes ilike '%15%' THEN agstempoexecucao ELSE 0 END as d15t,
			CASE WHEN agsperdetalhes ilike '%16%' THEN 1 ELSE 0 END as d16,
			CASE WHEN agsperdetalhes ilike '%16%' THEN agstempoexecucao ELSE 0 END as d16t,
			CASE WHEN agsperdetalhes ilike '%17%' THEN 1 ELSE 0 END as d17,
			CASE WHEN agsperdetalhes ilike '%17%' THEN agstempoexecucao ELSE 0 END as d17t,
			CASE WHEN agsperdetalhes ilike '%18%' THEN 1 ELSE 0 END as d18,
			CASE WHEN agsperdetalhes ilike '%18%' THEN agstempoexecucao ELSE 0 END as d18t,
			CASE WHEN agsperdetalhes ilike '%19%' THEN 1 ELSE 0 END as d19,
			CASE WHEN agsperdetalhes ilike '%19%' THEN agstempoexecucao ELSE 0 END as d19t,
			CASE WHEN agsperdetalhes ilike '%20%' THEN 1 ELSE 0 END as d20,
			CASE WHEN agsperdetalhes ilike '%20%' THEN agstempoexecucao ELSE 0 END as d20t,
			CASE WHEN agsperdetalhes ilike '%21%' THEN 1 ELSE 0 END as d21,
			CASE WHEN agsperdetalhes ilike '%21%' THEN agstempoexecucao ELSE 0 END as d21t,
			CASE WHEN agsperdetalhes ilike '%22%' THEN 1 ELSE 0 END as d22,
			CASE WHEN agsperdetalhes ilike '%22%' THEN agstempoexecucao ELSE 0 END as d22t,
			CASE WHEN agsperdetalhes ilike '%23%' THEN 1 ELSE 0 END as d23,
			CASE WHEN agsperdetalhes ilike '%23%' THEN agstempoexecucao ELSE 0 END as d23t
			FROM seguranca.agendamentoscripts 
			WHERE agsstatus='A' AND agsperiodicidade='diario') as foo";
	
	$cabecalho = array("00:00","01:00","02:00","03:00","04:00","05:00","06:00","07:00","08:00","09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00","17:00","18:00","19:00","20:00","21:00","22:00","23:00");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	?>	
	</td>
</tr>
<tr>
	<td>
	<p><b>Semanal</b></p>
	<?
	$sql = "SELECT '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=semanal&agsperdetalhes=1\">'||SUM(s1)||'<br>'||TO_CHAR((SUM(s1t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s1,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=semanal&agsperdetalhes=2\">'||SUM(s2)||'<br>'||TO_CHAR((SUM(s2t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s2,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=semanal&agsperdetalhes=3\">'||SUM(s3)||'<br>'||TO_CHAR((SUM(s3t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s3,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=semanal&agsperdetalhes=4\">'||SUM(s4)||'<br>'||TO_CHAR((SUM(s4t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s4,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=semanal&agsperdetalhes=5\">'||SUM(s5)||'<br>'||TO_CHAR((SUM(s5t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s5,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=semanal&agsperdetalhes=6\">'||SUM(s6)||'<br>'||TO_CHAR((SUM(s6t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s6,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=semanal&agsperdetalhes=0\">'||SUM(s7)||'<br>'||TO_CHAR((SUM(s7t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s7 
			FROM (
			SELECT 
			CASE WHEN agsperdetalhes ilike '%1%' THEN 1 ELSE 0 END as s1,
			CASE WHEN agsperdetalhes ilike '%1%' THEN agstempoexecucao ELSE 0 END as s1t,
			CASE WHEN agsperdetalhes ilike '%2%' THEN 1 ELSE 0 END as s2,
			CASE WHEN agsperdetalhes ilike '%2%' THEN agstempoexecucao ELSE 0 END as s2t,
			CASE WHEN agsperdetalhes ilike '%3%' THEN 1 ELSE 0 END as s3,
			CASE WHEN agsperdetalhes ilike '%3%' THEN agstempoexecucao ELSE 0 END as s3t,
			CASE WHEN agsperdetalhes ilike '%4%' THEN 1 ELSE 0 END as s4,
			CASE WHEN agsperdetalhes ilike '%4%' THEN agstempoexecucao ELSE 0 END as s4t,
			CASE WHEN agsperdetalhes ilike '%5%' THEN 1 ELSE 0 END as s5,
			CASE WHEN agsperdetalhes ilike '%5%' THEN agstempoexecucao ELSE 0 END as s5t,
			CASE WHEN agsperdetalhes ilike '%6%' THEN 1 ELSE 0 END as s6,
			CASE WHEN agsperdetalhes ilike '%6%' THEN agstempoexecucao ELSE 0 END as s6t,
			CASE WHEN agsperdetalhes ilike '%0%' THEN 1 ELSE 0 END as s7,
			CASE WHEN agsperdetalhes ilike '%0%' THEN agstempoexecucao ELSE 0 END as s7t
			FROM seguranca.agendamentoscripts 
			WHERE agsstatus='A' AND agsperiodicidade='semanal') as foo";
	
	$cabecalho = array("Segunda","Terça","Quarta","Quinta","Sexta","Sabado","Domingo");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	?>	
	</td>
</tr>
<tr>
	<td>
	<p><b>Mensal</b></p>
	<?
	$sql = "SELECT '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=1\">'||SUM(s1)||'<br>'||TO_CHAR((SUM(s1t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s1,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=2\">'||SUM(s2)||'<br>'||TO_CHAR((SUM(s2t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s2,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=3\">'||SUM(s3)||'<br>'||TO_CHAR((SUM(s3t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s3,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=4\">'||SUM(s4)||'<br>'||TO_CHAR((SUM(s4t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s4,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=5\">'||SUM(s5)||'<br>'||TO_CHAR((SUM(s5t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s5,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=6\">'||SUM(s6)||'<br>'||TO_CHAR((SUM(s6t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s6,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=7\">'||SUM(s7)||'<br>'||TO_CHAR((SUM(s7t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s7, 
				   '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=8\">'||SUM(s8)||'<br>'||TO_CHAR((SUM(s8t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s8,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=9\">'||SUM(s9)||'<br>'||TO_CHAR((SUM(s9t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s9,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=10\">'||SUM(s10)||'<br>'||TO_CHAR((SUM(s10t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s10,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=11\">'||SUM(s11)||'<br>'||TO_CHAR((SUM(s11t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s11,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=12\">'||SUM(s12)||'<br>'||TO_CHAR((SUM(s12t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s12,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=13\">'||SUM(s13)||'<br>'||TO_CHAR((SUM(s13t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s13,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=14\">'||SUM(s14)||'<br>'||TO_CHAR((SUM(s14t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s14,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=15\">'||SUM(s15)||'<br>'||TO_CHAR((SUM(s15t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s15,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=16\">'||SUM(s16)||'<br>'||TO_CHAR((SUM(s16t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s16,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=17\">'||SUM(s17)||'<br>'||TO_CHAR((SUM(s17t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s17,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=18\">'||SUM(s18)||'<br>'||TO_CHAR((SUM(s18t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s18,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=19\">'||SUM(s19)||'<br>'||TO_CHAR((SUM(s19t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s19,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=20\">'||SUM(s20)||'<br>'||TO_CHAR((SUM(s20t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s20,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=21\">'||SUM(s21)||'<br>'||TO_CHAR((SUM(s21t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s21,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=22\">'||SUM(s22)||'<br>'||TO_CHAR((SUM(s22t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s22,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=23\">'||SUM(s23)||'<br>'||TO_CHAR((SUM(s23t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s23,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=24\">'||SUM(s24)||'<br>'||TO_CHAR((SUM(s24t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s24,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=25\">'||SUM(s25)||'<br>'||TO_CHAR((SUM(s25t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s25,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=26\">'||SUM(s26)||'<br>'||TO_CHAR((SUM(s26t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s26,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=27\">'||SUM(s27)||'<br>'||TO_CHAR((SUM(s27t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s27,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=28\">'||SUM(s28)||'<br>'||TO_CHAR((SUM(s28t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s28,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=29\">'||SUM(s29)||'<br>'||TO_CHAR((SUM(s29t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s29,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=30\">'||SUM(s30)||'<br>'||TO_CHAR((SUM(s30t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s30,
			       '<a href=\"seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&agsperiodicidade=mensal&agsperdetalhes=31\">'||SUM(s31)||'<br>'||TO_CHAR((SUM(s31t)||' second')::interval, 'HH24:MI:SS')||'</a>' as s31
			FROM (
			SELECT 
			CASE WHEN agsperdetalhes ilike '%31%' THEN 1 ELSE 0 END as s31,
			CASE WHEN agsperdetalhes ilike '%31%' THEN agstempoexecucao ELSE 0 END as s31t,
			CASE WHEN agsperdetalhes ilike '%30%' THEN 1 ELSE 0 END as s30,
			CASE WHEN agsperdetalhes ilike '%30%' THEN agstempoexecucao ELSE 0 END as s30t,
			CASE WHEN agsperdetalhes ilike '%29%' THEN 1 ELSE 0 END as s29,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s29t,
			CASE WHEN agsperdetalhes ilike '%28%' THEN 1 ELSE 0 END as s28,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s28t,
			CASE WHEN agsperdetalhes ilike '%27%' THEN 1 ELSE 0 END as s27,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s27t,
			CASE WHEN agsperdetalhes ilike '%26%' THEN 1 ELSE 0 END as s26,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s26t,
			CASE WHEN agsperdetalhes ilike '%25%' THEN 1 ELSE 0 END as s25,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s25t,
			CASE WHEN agsperdetalhes ilike '%24%' THEN 1 ELSE 0 END as s24,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s24t,
			CASE WHEN agsperdetalhes ilike '%23%' THEN 1 ELSE 0 END as s23,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s23t,
			CASE WHEN agsperdetalhes ilike '%22%' THEN 1 ELSE 0 END as s22,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s22t,
			CASE WHEN agsperdetalhes ilike '%21%' THEN 1 ELSE 0 END as s21,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s21t,
			CASE WHEN agsperdetalhes ilike '%20%' THEN 1 ELSE 0 END as s20,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s20t,
			CASE WHEN agsperdetalhes ilike '%19%' THEN 1 ELSE 0 END as s19,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s19t,
			CASE WHEN agsperdetalhes ilike '%18%' THEN 1 ELSE 0 END as s18,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s18t,
			CASE WHEN agsperdetalhes ilike '%17%' THEN 1 ELSE 0 END as s17,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s17t,
			CASE WHEN agsperdetalhes ilike '%16%' THEN 1 ELSE 0 END as s16,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s16t,
			CASE WHEN agsperdetalhes ilike '%15%' THEN 1 ELSE 0 END as s15,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s15t,
			CASE WHEN agsperdetalhes ilike '%14%' THEN 1 ELSE 0 END as s14,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s14t,
			CASE WHEN agsperdetalhes ilike '%13%' THEN 1 ELSE 0 END as s13,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s13t,
			CASE WHEN agsperdetalhes ilike '%12%' THEN 1 ELSE 0 END as s12,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s12t,
			CASE WHEN agsperdetalhes ilike '%11%' THEN 1 ELSE 0 END as s11,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s11t,
			CASE WHEN agsperdetalhes ilike '%10%' THEN 1 ELSE 0 END as s10,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s10t,
			CASE WHEN agsperdetalhes ilike '%9%' THEN 1 ELSE 0 END as s9,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s9t,
			CASE WHEN agsperdetalhes ilike '%8%' THEN 1 ELSE 0 END as s8,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s8t,
			CASE WHEN agsperdetalhes ilike '%7%' THEN 1 ELSE 0 END as s7,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s7t,
			CASE WHEN agsperdetalhes ilike '%6%' THEN 1 ELSE 0 END as s6,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s6t,
			CASE WHEN agsperdetalhes ilike '%5%' THEN 1 ELSE 0 END as s5,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s5t,
			CASE WHEN agsperdetalhes ilike '%4%' THEN 1 ELSE 0 END as s4,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s4t,
			CASE WHEN agsperdetalhes ilike '%3%' THEN 1 ELSE 0 END as s3,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s3t,
			CASE WHEN agsperdetalhes ilike '%2%' THEN 1 ELSE 0 END as s2,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s2t,
			CASE WHEN agsperdetalhes ilike '%1%' THEN 1 ELSE 0 END as s1,
			CASE WHEN agsperdetalhes ilike '%29%' THEN agstempoexecucao ELSE 0 END as s1t
			FROM seguranca.agendamentoscripts 
			WHERE agsstatus='A' AND agsperiodicidade='mensal') as foo";
	
	$cabecalho = array("1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	?>	
	</td>
</tr>
<tr>
	<td class="SubTituloCentro"><input type="button" name="vertodos" value="Ver todos" onclick="window.location='seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A';"></td>
</tr>
</table>
<?php

if($_REQUEST['agsperiodicidade']) {
	$wh[] = "agsperiodicidade='".$_REQUEST['agsperiodicidade']."'";
}

if($_REQUEST['agsperdetalhes']) {
	$wh[] = "agsperdetalhes ilike '%".$_REQUEST['agsperdetalhes']."%'";
}

$sql = "SELECT
    '<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.location=\'seguranca.php?modulo=principal/agendamentoscriptsgerenciar&acao=A&agsid='||agsid||'\'\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"var conf=confirm(\'Deseja realmente excluir?\');if(conf){window.location=\'seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&requisicao=deletarAgendamentoScripts&agsid='||agsid||'\';}\"></center>' as alterar, agsfile, agsdescricao, CASE WHEN agsperiodicidade='diario' THEN 'Diário'
                                                                                                            WHEN agsperiodicidade='semanal' THEN 'Semanal' 
                                                                                                            WHEN agsperiodicidade='mensal' THEN 'Mensal' END as per,
                                                                                                            TO_CHAR((agstempoexecucao||' second')::interval, 'HH24:MI:SS') as tempo,
                                                                                                            to_char(agsdataexec,'dd/mm/YYYY') as ultexec,
                                                                                                            agsperdetalhes
		FROM seguranca.agendamentoscripts WHERE agsstatus='A'".(($wh)?" AND ".implode(" AND ",$wh):"")." ORDER BY agsfile";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $ar) {
		$det = explode(";",$ar['agsperdetalhes']);
		unset($detalhes);
		switch($ar['per']) {
			case 'Mensal':
				if($det) {
					foreach($det as $t) {
						$detalhes[] = $t;
					}
				}
				break;
			case 'Diário':
				if($det) {
					foreach($det as $t) {
						$detalhes[] = "- ".$t.":00";
					}
				}
				break;
			case 'Semanal':
				if($det) {
					foreach($det as $t) {
						switch($t) {
							case 0: $detalhes[] = "- Domingo";break;
							case 1: $detalhes[] = "- Segunda-feira";break;
							case 2: $detalhes[] = "- Terça-feira";break;
							case 3: $detalhes[] = "- Quarta-feira";break;
							case 4: $detalhes[] = "- Quinta-feira";break;
							case 5: $detalhes[] = "- Sexta-feira";break;
							case 6: $detalhes[] = "- Sábado";break;
						}
						
					}
				}
				break;
		}
		$ar['link'] = ((is_file('./scripts_exec/'.$ar['agsfile']))?"<font color=blue>Sim</font>":"<font color=red>Não</font>");
		$ar['agsperdetalhes'] = '<span style=font-size:x-small;width:400px;height:80px;overflow:auto;>'.(($detalhes)?implode("<br>",$detalhes):"Sem detalhes").'</span>';
		$regs[] = $ar;
		
	}
	
} else {
	$regs[] = array();
}
?>
<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'><b>Os arquivos cadastrados devem estar na pasta : /var/www/simec/seguranca/www/scripts_exec/</b></td>
</tr>
<tr>
	<td class='SubTituloDireita'><input type="button" name="inseriragendamento" value="Inserir" onclick="window.location='seguranca.php?modulo=principal/agendamentoscriptsgerenciar&acao=A';"></td>
</tr>
</table>
<?php 
$cabecalho = array("&nbsp;", "Arquivo", "Resumo", "Periodicidade", "Tempo execução", "Última execução","Detalhes", "Existe?");

$db->monta_lista($regs,$cabecalho,300,5,'N','center',$par2);

?>

<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'><b>Os arquivos cadastrados devem estar na pasta : /var/www/simec/seguranca/www/scripts_exec/</b></td>
</tr>
<tr>
	<td class='SubTituloDireita'><input type="button" name="inseriragendamento" value="Inserir" onclick="window.location='seguranca.php?modulo=principal/agendamentoscriptsgerenciar&acao=A';"></td>
</tr>
</table>