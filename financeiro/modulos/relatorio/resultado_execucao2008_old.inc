<?

ini_set( "memory_limit", "256M" );
set_time_limit(0);

// Parâmetros para a nova conexão com o banco do SIAFI
$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

// Cria o novo objeto de conexão
$db2 = new cls_banco();

$arrResultadoRelatorio = array(
						array(
							"anexo" => "Anexo I (NCA+NIA) - Demais não incluídas nas classificações abaixo¹",
							"mes" => "mes = '12'",
							"unidade_orcamentaria" => "26101-26352",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) not in ('95','96','81','50','82','79')",
							"projeto_atividade" => "acacod not in ('0E36','0005','0022','0716','2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod not in ('0681')",
							"programa_trabalho" => "funcod || sfucod || prgcod || acacod || loccod not in ('12364107300480553','12364107385510101','12361106105090105')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "Anexo IV (NXA+NXB) - Programa 0681",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26352",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"projeto_atividade" => "acacod not in ('0E36','0005','0022','0716','2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "Obrigatórias(NOA) - PNAE, PNATE, EJA, PDDE, Aux Alim e Aux Transp",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26352",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) not in ('95','96','81','50','82','79')",
							"projeto_atividade" => "acacod in ('2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "Anexo II (NCB+NIB) - Fte 50",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26352",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) in ('50')",
							"projeto_atividade" => "acacod not in ('0E36','0005','0022','0716','2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "Anexo III (NCC+NIC) - Fte 82",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26352",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) in ('82','79')",
							"projeto_atividade" => "acacod not in ('0E36','0005','0022','0716','2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "FUNDEB - Ação 0E36",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26999",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) not in ('95','96','81','50','82','79')",
							"projeto_atividade" => "acacod in ('0E36')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "Convênios(81) e Doações(95 e 96)",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26352",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) in ('95','96','81')",
							"projeto_atividade" => "acacod not in ('0E36','0005','0022','0716','2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "Precatórios - Ações 0005, 0022 e 0716",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26352",
							"fonte_recursos" => "substr(foncod, 3, 2) not in ('95','96','81','50','82','79')",
							"projeto_atividade" => "acacod in ('0005','0022','0716')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "Pessoal e Encargos Sociais - GND 1",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26352",
							"grupo_despesa" => "gndcod in ('1')",
							"fonte_recursos" => "substr(foncod, 3, 2) not in ('95','96','81','50','82','79')",
							"projeto_atividade" => "acacod not in ('0E36','0005','0022','0716','2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "Serviços da Dívida - GND 2 e 6",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26352",
							"grupo_despesa" => "gndcod in ('2','6')",
							"projeto_atividade" => "acacod not in ('0E36','0005','0022','0716','2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "<strong>Créditos Extraordinários</strong> - Abertos e Reabertos",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "26101-26999",
							"programa_trabalho" => "funcod || sfucod || prgcod || acacod || loccod in ('12364107300480553','12364107385510101','12361106105090105')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "12.123.1073.4556.0001 - Administração FIES (LOA)",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "unicod in ('74902')",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) not in ('95','96','81','50','82','79')",
							"projeto_atividade" => "acacod in ('4556')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "12.846.1073.0579.0001 - Concessão de Financiamento ao Estudante - FIES (LOA)",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "unicod in ('74902')",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) not in ('95','96','81','50','82','79')",
							"projeto_atividade" => "acacod in ('0579')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							),
						array(
							"anexo" => "28.845.0903.0369.0001 - Cota Parte do Salário Educação",
							"mes" => "mes = '11'",
							"unidade_orcamentaria" => "unicod in ('73107')",
							"grupo_despesa" => "gndcod not in ('1','2','6')",
							"fonte_recursos" => "substr(foncod, 3, 2) not in ('95','96','81','50','82','79')",
							"projeto_atividade" => "acacod not in ('0E36','0005','0022','0716','2011','2012','0515','0969','8744','8790')",
							"programa" => "prgcod not in ('0681')",
							"colunas" => array("sldcontacontabil = '192520100'",
											   "sldcontacontabil = '293110699' AND ungcod = '150014'",
											   "sldcontacontabil in ('192110101','192190301','192110209','192190302','192140200','192140100','192130201','192110303','192110301','192110201','192190209','192130101','192130102','192190201')",
											   "sldcontacontabil in ('292130201','292130202','292130209','292130301','292130100')",
											   "Saldo de Limite não Empenhado",
											   "Emp x Limite(%)",
											   "sldcontacontabil in ('293110601','293110602')",
											   "% Limite Utilizado")
							)
						);
//dbg($arrResultadoRelatorio);
/*$cor = "#f4f4f4";
		
$count++;

$nome = "linha_".$itcid;

if ($count % 2){
	$cor = "#e0e0e0";
}*/
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" width="50" rowspan="2">
			<img src="../imagens/brasao.gif" width="45" height="45" border="0">
		</td>
		<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				<?php echo NOME_SISTEMA; ?><br/>
				Acompanhamento da Execução Orçamentária<br/>
				MEC / SE - Secretaria Executiva <br />
				SPO - Subsecretaria de Planejamento e Orçamento
			</font>
		</td>
		<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?>
			</font>
		</td>
		<tr><td>&nbsp;</td></tr>
		<tr>
			<td colspan="3" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:18px;">Execução 2008</font></b>
			</td>
		</tr>
	</tr>
</table>
<table style="border-bottom: 2px solid #000000;" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Anexos</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PLOA 2009</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Limites para Empenho</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Dotação Autorizada (Lei + Créditos)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Empenhos</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Saldo de Limite não Empenhado</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Emp. x Limite (%)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Cota Limite a Utilizar</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>% Limite Utilizado</strong></td>
		</tr>
	</thead>
	<tbody>
		<?
			$subtotal = array();
			$total = array();
			for($i=0; $i<count($arrResultadoRelatorio); $i++) {
				echo "<tr bgcolor=\"#f9f9f9\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#f9f9f9';\">";
				echo "<td align=\"left\">".$arrResultadoRelatorio[$i]["anexo"]."</td>";
				
				$limiteEmpenho = $empenho = $cotaLimite = 0;
				for($j=0; $j<count($arrResultadoRelatorio[$i]["colunas"]); $j++) {
					// Saldo de Limite Não Empenhado => (limite para empenho - empenho)
					if($j==4) {
						$resultado = ($limiteEmpenho - $empenho);
					}
					// Emp. x Limite(%) => (empenho / limite para empenho) * 100
					else if($j==5) {
						$resultado = ($limiteEmpenho != 0) ? (($empenho / $limiteEmpenho) * 100) : 0;
					}
					// % Limite Utilizado => (1 - (cota limite a utilizar / limite para empenho)) * 100
					else if($j==7) {
						$resultado = ($limiteEmpenho != 0) ? ((1 - ($cotaLimite / $limiteEmpenho)) * 100) : 0;
					}
					// Nas outras colunas pega o valor do banco...
					else {
						$sql = "SELECT
									sum(sldvalor)
								FROM
									dw.saldo
								WHERE";
						
						if($arrResultadoRelatorio[$i]["mes"]) {
							//$sql .= " ".$arrResultadoRelatorio[$i]["mes"]." AND ";
						}
						if($arrResultadoRelatorio[$i]["unidade_orcamentaria"]) {
							if(strpos($arrResultadoRelatorio[$i]["unidade_orcamentaria"], "-")) {
								$uo = explode("-", $arrResultadoRelatorio[$i]["unidade_orcamentaria"]);
								$sql .= " (unicod BETWEEN '".$uo[0]."' AND '".$uo[1]."') AND ";
							}
							else {
								$sql .= " ".$arrResultadoRelatorio[$i]["unidade_orcamentaria"]." AND ";
							}
						}
						if($arrResultadoRelatorio[$i]["grupo_despesa"]) {
							$sql .= " ".$arrResultadoRelatorio[$i]["grupo_despesa"]." AND ";
						}
						if($arrResultadoRelatorio[$i]["fonte_recursos"]) {
							$sql .= " ".$arrResultadoRelatorio[$i]["fonte_recursos"]." AND ";
						}
						if($arrResultadoRelatorio[$i]["projeto_atividade"]) {
							$sql .= " ".$arrResultadoRelatorio[$i]["projeto_atividade"]." AND ";
						}
						if($arrResultadoRelatorio[$i]["programa"]) {			
							$sql .= " ".$arrResultadoRelatorio[$i]["programa"]." AND ";
						}
						if($arrResultadoRelatorio[$i]["programa_trabalho"]) {
							$sql .= " ".$arrResultadoRelatorio[$i]["programa_trabalho"]." AND ";
						}
						if($arrResultadoRelatorio[$i]["colunas"][$j]) {
							$sql .= $arrResultadoRelatorio[$i]["colunas"][$j];
						}
									
						$resultado = $db2->pegaUm($sql);
						
						$total[$j] += $resultado;
						$subtotal[$j] += $resultado;
						
						if($j==1) { $limiteEmpenho = $resultado; }
						if($j==3) { $empenho = $resultado; }
						if($j==6) { $cotaLimite = $resultado; }
					}
					
					echo "<td align=\"right\">".number_format($resultado, 2, ',', '.')."</td>";
				}
								
				echo "<tr>";
				
				if(($i==2) || ($i==4) || ($i==9) || ($i==10) || ($i==12) || ($i==13)) {
					echo "<tr bgcolor=\"#BFBFBF\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#BFBFBF';\">";
					echo "<td align=\"right\"><strong>Subtotal</strong></td>";
					for($j=0; $j<count($arrResultadoRelatorio[$i]["colunas"]); $j++) {
						echo "<td align=\"right\"><strong>".number_format($subtotal[$j], 2, ',', '.')."</strong></td>";
						$subtotal[$j] = 0;
					}
					echo "</tr>";
					
					if($i==9) {
						echo "<tr bgcolor=\"#FFF79F\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#FFF79F';\">";
						echo "<td align=\"right\"><strong>Total(Discricionárias + Obrigatórias)</strong></td>";
						for($j=0; $j<count($arrResultadoRelatorio[$i]["colunas"]); $j++) {
							echo "<td align=\"right\"><strong>".number_format($total[$j], 2, ',', '.')."</strong></td>";
						}
						echo "</tr>";
					}
					
					if($i==10) {
						echo "<tr bgcolor=\"#AFCCFF\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#AFCCFF';\">";
						echo "<td align=\"center\"><strong>Total MEC</strong></td>";
						for($j=0; $j<count($arrResultadoRelatorio[$i]["colunas"]); $j++) {
							echo "<td align=\"right\"><strong><font color=\"red\">".number_format($total[$j], 2, ',', '.')."</font></strong></td>";
						}
						echo "</tr>";
					}
					
					if($i==13) {
						echo "<tr bgcolor=\"#AFCCFF\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#AFCCFF';\">";
						echo "<td align=\"center\"><strong>Total MEC + FIES + Cota Parte do Salário Educação</strong></td>";
						for($j=0; $j<count($arrResultadoRelatorio[$i]["colunas"]); $j++) {
							echo "<td align=\"right\"><strong><font color=\"red\">".number_format($total[$j], 2, ',', '.')."</font></strong></td>";
						}
						echo "</tr>";
					}
				}
			}
		?>
	</tbody>
</table>
<br />
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left"><strong><font style="font-size:11px;">Notas:</font></strong></td>
	</tr>
	<tr>
		<td align="left"><font style="font-size:11px;">1. No limite para empenho do Anexo I (NCA e NIA), foi deduzido o valor de R$105.000.000 referente à transferência para o FIES.</font></td>
	</tr>
	<tr>
		<td align="left"><font style="font-size:11px;">2. Os créditos Extraordinários e Eseciais poder ser reabertos no exercício  de 2008 pelo saldo não executado.</font></td>
	</tr>
</table>