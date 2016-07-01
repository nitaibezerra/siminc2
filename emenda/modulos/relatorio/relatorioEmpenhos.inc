<?php 
set_time_limit(0);
ini_set('memory_limit', '1024M');

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print '<br/>
			<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
			<table  align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td style="height: 20px;"></td>
				</tr>
				<tr>
					<td style="text-align: center;" class="div_rolagem">
						<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
					</td>
				</tr>
			</table>
			</form>
			<script type="text/javascript">				
				function exportarExcel(){
					document.getElementById(\'exporta\').value = "true";
					document.getElementById(\'formulario\').submit();
				}
				//$(\'loader-container\').show();
			</script>';
			
$sql  = "select distinct
			pt.ptrid || '/' || pt.ptrexercicio as numpta,			
			fup.fupfuncionalprogramatica,
			vede.emecod,
			case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
			ent.enbcnpj,
			ent.enbnome,
			mun.mundescricao,
			ent.estuf,
			ef.exfvalor as valor,
			CASE WHEN exfespecieempenho = '01' THEN 'Original'
		         WHEN exfespecieempenho = '02' THEN 'Reforço'
		         WHEN exfespecieempenho = '03' THEN 'Anulação'
		    END AS especieempenho,
			ef.exfnumsolempenho,
			ef.exfnumempenhooriginal,
			to_char(exfdatainclusao, 'DD/MM/YYYY HH24:mi:ss') as exfdatainclusao,
			se.semcod || ' - ' ||se.semdesc as situacao,
			(SELECT  
                    CASE WHEN  count(1) = 1 THEN 'Pendente'
                    WHEN  ef.exfstatus = 'A' THEN 'Ativo'
                    WHEN  ef.exfstatus = 'I' THEN 'Inativo'
                    END as status 
                  FROM emenda.execucaofinanceira  WHERE exfidpai = ef.exfid and exfstatus = 'A')  as status
            
			
		from emenda.planotrabalho pt
			inner join emenda.ptiniciativa pti on pti.ptrid = pt.ptrid
			inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
			inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid
			inner join emenda.ptemendadetalheentidade ped on ped.pedid = per.pedid
			inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
			inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
			inner join emenda.autor aut on aut.autid = vede.autid
			inner join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
			inner join emenda.execucaofinanceira ef on pti.ptrid = ef.ptrid and ped.pedid = ef.pedid and ef.exfstatus = 'A'
			inner join emenda.situacaoempenho se on ef.semid = se.semid
			inner join monitora.pi_planointerno pli on pli.plicod = ef.plicod
            inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
            inner join monitora.ptres ptr on ptr.ptrid = plpt.ptrid and ptr.ptres = ef.ptres
			inner join public.esfera esf on fup.esfcod = esf.esfcod
			inner join emenda.entidadebeneficiada ent on pt.enbid = ent.enbid
			INNER JOIN territorios.municipio mun ON mun.muncod = ent.muncod 
		WHERE
			pt.ptrexercicio = ".$_SESSION['exercicio']." --Verifica o ano de exercicio! Victor Benzi
			
		group by
			pt.ptrid,
			pt.ptrexercicio,
			ef.exfid,
			fup.fupfuncionalprogramatica,
			vede.emecod,
			aut.autnome,
			vede.emerelator,
			ef.exfvalor,
			ef.exfnumsolempenho,
			especieempenho,
			ef.exfnumempenhooriginal,
			exfdatainclusao,
			situacao,
			pervalor,
			ef.exfstatus,
			status,
			ent.enbcnpj,
			ent.enbnome,
			ent.estuf,
			mun.mundescricao
			
		order by exfdatainclusao
		";

$cabecalho 	    = array("Nº PTA", "Funcional", "Código", "Autor", "CNPJ", "Nome Entidade", "Munícipio", "UF", "Valor", "Espécie Empenho", "Num Sol. Empenho", "Num Empenho Original", "Data Inclusão", "Situação", "Status");

if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');

	$db->sql_to_excel($sql, 'relEmendasEmpenho', $cabecalho);
	exit;
	$exporta = "false";
}

print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" class="subtitulocentro" >Relatório de Empenho</td></tr></table>';
 
$tamanho		= array('5%', '7%', '5%', '10%', '7%', '15%', '10%', '2%', '7%', '5%', '5%', '7%', '5%', '15%', '10%' );															
$alinhamento	= array('left', 'left', 'right', 'center', 'right', 'center', 'right', 'center', 'center', 'center', 'center' );													
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);

?>