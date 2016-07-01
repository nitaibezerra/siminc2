<?php

function monta_sql(){
	global $db;
	
	extract($_REQUEST);

	$where = array();
	
	// estado
	if( $estuf[0] && $estuf_campo_flag != '' ){
		array_push($where, "mun.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "')");
	}
	
	// municipio
	if( $muncod[0] && $muncod_campo_flag != '' ){
		array_push($where, "mun.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "')");
	}
	

	$sql = "SELECT
					o.obrid as id,
					o.preid as preid,
					obrnome as descricao,
					'( ' || TO_CHAR(emi.emicnpj::int8, '00\".\"000\".\"000\"/\"0000\"-\"00') || ' ) ' || emi.emidsc AS emidsc,
					entnome as unidade_implantadora,
					mun.mundescricao||' - '||mun.estuf as municipio,
					TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio,
					TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino,
					ed.esddsc as situacao,
					CASE 
						WHEN sup.supdtinclusao IS NOT NULL AND (staid IN (3, 6, 7)) THEN
							to_char(sup.supdtinclusao, 'DD/MM/YYYY')||' ( '||DATE_PART('days', NOW() - sup.supdtinclusao)||' dia(s) )'
						WHEN sup.supdtinclusao IS NOT NULL AND staid NOT IN (3, 6, 7, 4, 5, 99) THEN
							to_char(sup.supdtinclusao, 'DD/MM/YYYY')||' ( '||DATE_PART('days', NOW() - sup.supdtinclusao)||' dia(s) )'
						WHEN staid IN (1, 2) THEN
							CASE WHEN obrdtultvistoria IS NOT NULL THEN 
								to_char(obrdtultvistoria, 'DD/MM/YYYY')||' ( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
							 ELSE 
								to_char(obrdtinclusao, 'DD/MM/YYYY')||' ( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )' 
							END 
						 WHEN staid IN (3) THEN
						  	COALESCE(to_char(obrdtultvistoria, 'DD/MM/YYYY'), to_char(obrdtinclusao, 'DD/MM/YYYY'))||' ( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
						 ELSE
							CASE WHEN obrdtultvistoria IS NOT NULL THEN 
									to_char(obrdtultvistoria, 'DD/MM/YYYY')||' ( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obrdtinclusao, 'DD/MM/YYYY')||' ( '||DATE_PART('days', NOW() - obrdtinclusao)||' dia(s) )' 
							END 
					END AS ultima_atualizacao,
					to_char( obrdtultvistoria,'DD/MM/YYYY') as dtvistoria,
					responsavel,
					obrpercentultvistoria,
					tpo.tpodsc,
					ocrvalorexecucao,
					COALESCE('CEP '||ende.endcep,'CEP não preenchido')||', '||COALESCE(trim(ende.endlog),'Logradouro não preenchido')||', N '||trim(ende.endnum)||' '||COALESCE(trim(ende.endcom),'')||', '||COALESCE(trim(ende.endbai),'Bairro não preenchido')||', '||mun.mundescricao||', '||mun.estuf as endereco
				FROM
					obras2.obras o
				INNER JOIN entidade.endereco		ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = 4
				LEFT JOIN obras2.empresami_uf 		euf ON euf.estuf = ende.estuf AND euf.eufstatus = 'A' 
				LEFT JOIN obras2.empresami emi ON 	emi.emiid = euf.emiid AND emi.emistatus = 'A' 																	
				LEFT JOIN territorios.municipio 	mun ON mun.muncod = ende.muncod
								
				LEFT JOIN workflow.documento 		  d ON d.docid = o.docid AND tpdid = 126
				LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = d.esdid
					
				LEFT JOIN entidade.entidade 		ent ON ent.entid = o.entid
				LEFT JOIN obras2.tipologiaobra 		tpo ON tpo.tpoid = o.tpoid
				LEFT JOIN obras2.empreendimento 	  e ON e.empid = o.empid AND e.empstatus = 'A'
				LEFT JOIN obras2.obrascontrato 		 oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'																     
				LEFT JOIN (
							SELECT
								max( res_estado ) as res_estado,
								obrid
							FROM
							(
							SELECT DISTINCT
								CASE 
									WHEN rstdtsuperacao IS NOT NULL THEN 1
									WHEN rstdtprevisaoregularizacao > now() THEN 2
									WHEN rstdtprevisaoregularizacao < now() THEN 3
									ELSE 2
								END as res_estado,
								obrid
							FROM
								obras2.restricao
							WHERE
								obrid IS NOT NULL AND 
								rstitem = 'R' AND
								rststatus = 'A' 
							) as  foo
							GROUP BY
								obrid
							) res ON res.obrid = o.obrid
				LEFT JOIN (
							SELECT
								max( inc_estado ) as inc_estado,
								obrid
							FROM
								(
								SELECT DISTINCT
									CASE 
										WHEN rstdtsuperacao IS NOT NULL THEN 1
										WHEN rstdtprevisaoregularizacao > now() THEN 2
										WHEN rstdtprevisaoregularizacao < now() THEN 3
										ELSE 2
									END as inc_estado,
									obrid
								FROM
									obras2.restricao
								WHERE
									obrid IS NOT NULL AND 
									rstitem = 'I' AND
									rststatus = 'A' 
								) as  foo
							GROUP BY
								obrid
							) inc ON inc.obrid = o.obrid
				LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid
				
				WHERE
					o.obrstatus = 'A'
					 AND o.obridpai IS NULL AND e.orgid IN(3) AND o.tpoid IN (104, 105) 
					 ".(($where)?" AND ".implode(" AND ",$where):"")."
				ORDER BY
					2";
	
	return $sql;
}


// exibe consulta
if($_REQUEST['tiporelatorio']){
	switch($_REQUEST['tiporelatorio']) {
		case 'xls':
			$sql = monta_sql();

			$cabecalho				 = array("ID", "ID Pré-Obra", "Obra", "Empresa", "Unidade Implantadora", "Município", 
										 "Data de Início da Execução", "Data de Término da Execução", "Situação da Obra", 
										 "Última Atualização", "Última Vistoria", "Realizado por", "% Executado", "Tipologia", 
										 "Valor Contrato","Endereço");
			
			$db->sql_to_xml_excel($sql,'rel_obrasmi', $cabecalho);
			exit;
			
	}
	
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
monta_titulo( "Relatório Obras MI", 'Selecione os filtros' );

?>


<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>


<script type="text/javascript">

	
function exibeRelatorioGeral(tipo){
	
	var formulario = document.formulario;
	
    selectAllOptions(document.getElementById('estuf'));
    selectAllOptions(document.getElementById('muncod'));
    
	formulario.action = 'obras2.php?modulo=relatorio/relatorioobrasmi&acao=A&tiporelatorio='+tipo;
	window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';

	
	formulario.submit();
	
}

	
/* Função para substituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}
				
/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}
	
/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
	
	
//-->
</script>


<form action="" method="post" name="formulario" id="filtro"> 

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<?php

$stSql = "SELECT
				estuf AS codigo,
				estuf || ' / ' || estdescricao AS descricao
			FROM 
				territorios.estado
			ORDER BY
				2 ";
mostrarComboPopup( 'UFs:', 'estuf',  $stSql, '', 'Selecione a(s) UF(s)' );

$stSql = "SELECT
				muncod AS codigo,
				estuf || ' / ' || mundescricao AS descricao
			FROM 
				territorios.municipio
			ORDER BY
				2 ";
mostrarComboPopup( 'Municípios:', 'muncod',  $stSql, '', 'Selecione o(s) Município(s)' );				


?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				 <!-- <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/> -->
				 <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>

</form>