<?php 
require_once '_funcoes_termoCooperacao.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
monta_titulo( "Relatório por Unidade Gestora", 'Selecione o filtro e/ou agrupador desejado' );
?>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>
<table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr id="tr_unidade">	
				<?php 
				$stSql = "SELECT ungcod as codigo, ungcod ||' - '|| ungdsc as descricao FROM public.unidadegestora
   where ungcod in ('150011','150016','152389','152390','150028','150002','150019','150004')";
				mostrarComboPopup( 'Unidade Gestora:', 'ungcod',  $stSql, '', 'Selecione a(s) UG(s)' );
					
				?>
			</tr>
			<tr>
				<td class="subtitulodireita" >&nbsp;</td>
				<td lign="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="exibeRelatorioGeral('visual');"/>
					<input type="button" name="Gerar Relatório" value="Visualizar XLS"  onclick="exibeRelatorioGeral('xls');"/>
				</td>
			</tr>
</table>
</form>
<?php
if($_POST['tipo_relatorio']){
	if($_POST['ungcod'][0] != null || $_POST['ungcod'][0] != ''){
	$unidades = $_POST['ungcod'];
	$where = "and tcp.ungcodconcedente in (";
	foreach ($unidades as $dados){
	  $where .= "'".$dados."',";
	}
	$whereG = substr($where, 0, -1);
	$whereG .= ")";
}else{
	$whereG = "and tcp.ungcodconcedente in ('150011','150016','152389','152390','150028','150002','150019','150004')";
}
global $db;

$sql = "SELECT 
	numero_termo,
	unidade_proponente,
	unidade_concedente,
	resp_proponente,
	resp_concedente,
	situacao,
	coordenacao,
	tcpdscobjetoidentificacao,
	ano_referencia,
	sum(valor) as total
FROM (
	SELECT DISTINCT 	       
	       foo.tcpid as numero_termo,
	       foo.unidadegestorap as unidade_proponente,
	       foo.unidadegestorac as unidade_concedente,
	       case when foo.altprop is not null then nomerpc else coalesce(coalesce(foo.prop, foo.nomerpc), ' - ') end as resp_proponente,
	       coalesce(coalesce(conc , foo.nomerpp), ' - ') as resp_concedente,
	       foo.esddsc as situacao,
	       foo.coodsc as coordenacao,
	       tcpdscobjetoidentificacao,
	       pro1.proanoreferencia as ano_referencia,
	       pro1.valor as valor	
	FROM (			
			SELECT DISTINCT 
					tcp.tcpid,						
					unp.ungcod || ' / ' || unp.ungdsc || ' - ' || unp.ungabrev as unidadegestorap,
					unc.ungcod || ' / ' || unc.ungdsc || ' - ' || unc.ungabrev as unidadegestorac,					
					rpc.nome as nomerpc,
					rpp.nome as nomerpp,
					( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid=1597 and hd.docid = tcp.docid order by hstid desc limit 1 ) as prop,
					( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid in (1612, 2442) and hd.docid = tcp.docid order by hstid desc limit 1 ) as conc,
					( SELECT us.usunome FROM workflow.historicodocumento hd inner join seguranca.usuario us on us.usucpf = hd.usucpf where hd.aedid=1620 and hd.docid = tcp.docid order by hstid desc limit 1 ) as altprop,									
					esd.esddsc as esddsc
					,coalesce(cdn.coodsc, '-') as coodsc,
					tcpdscobjetoidentificacao
			FROM monitora.termocooperacao tcp	

			LEFT JOIN elabrev.coordenacao cdn 
				ON cdn.cooid = tcp.cooid
			LEFT JOIN public.unidadegestora unp 
				ON unp.ungcod = tcp.ungcodproponente			
			LEFT JOIN public.unidadegestora unc 
				ON unc.ungcod = tcp.ungcodconcedente
			LEFT JOIN elabrev.representantelegal rpp
				ON rpp.ug = tcp.ungcodconcedente
			LEFT JOIN elabrev.representantelegal rpc
				ON rpc.ug = tcp.ungcodproponente
			LEFT JOIN workflow.documento doc  
				ON doc.docid = tcp.docid
			LEFT JOIN workflow.estadodocumento esd  
				ON esd.esdid = doc.esdid
						
			WHERE tcpstatus = 'A' 
			AND tcp.tcpid in (select distinct tc.tcpid from monitora.termocooperacao tc
				left join monitora.previsaoorcamentaria po on tc.tcpid = po.tcpid
				where (po.proanoreferencia >= {$_SESSION['exercicio']} or po.proanoreferencia is null) and tcpstatus = 'A')			
 {$whereG}
	) as foo
	left join (
		SELECT DISTINCT
			pro.tcpid,
			pro.proid,
			ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
			substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
			substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
			pro.ptrid,
			a.acacod,
			pro.pliid,
			case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
			pro.ndpid,
			to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
			coalesce(pro.provalor, 0) as valor,
			crdmesliberacao,
			crdmesexecucao,
			pro.proid,
			pro.proanoreferencia,
			pro.prodata
		FROM monitora.previsaoorcamentaria pro
		LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
		LEFT JOIN monitora.pi_planointernoptres pts 	ON pts.pliid = pi.pliid
		LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
		LEFT JOIN monitora.ptres p 			ON p.ptrid = pro.ptrid
		LEFT JOIN monitora.acao a 			ON a.acaid = p.acaid
		LEFT JOIN public.unidadegestora u 		ON u.unicod = p.unicod
		LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
		WHERE pro.prostatus = 'A'
		--AND pro.proanoreferencia IS NOT NULL
		--AND crdmesliberacao IS NOT NULL
		--AND crdmesexecucao IS NOT NULL
	) as pro1 on pro1.tcpid = foo.tcpid
	order by foo.tcpid, pro1.proanoreferencia
) AS foa
GROUP BY numero_termo,unidade_proponente,unidade_concedente,resp_proponente,resp_concedente,situacao,
coordenacao,tcpdscobjetoidentificacao,ano_referencia
ORDER BY numero_termo, ano_referencia";

	if ($_POST['tipo_relatorio'] == 'visual'){
			$cabecalho = array("Numero do Termo", "Unidade Proponente", "Unidade Concedente", "Responsável Proponente", "Responsável Concedente", "Situação",
			"Coordenação", "Objeto", "Ano Referência","Valor Total");
			$db->monta_lista($sql,$cabecalho,100000,5,'N','100%','');
	}
	switch($_POST['tipo_relatorio']) {
		case 'xls':
			//$sql = monta_sql();
			ob_clean();
			header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
			header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
			header ( "Pragma: no-cache" );
			header ( "Content-type: application/xls; name=rel_unidadegestora_".date("Ymdhis").".xls");
			header ( "Content-Disposition: attachment; filename=rel_unidadegestora_".date("Ymdhis").".xls");
			header ( "Content-Description: MID Gera excel" );
				
			$cabecalho = array("Numero do Termo", "Unidade Proponente", "Unidade Concedente", "Responsável Proponente", "Responsável Concedente", "Situação",
					"Coordenação", "Objeto", "Ano Referência",
					"Valor Total");
				
				
			$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%','');
			exit;
			
}
	}

?>
<script type="text/javascript">
document.getElementById('aguarde').style.display = 'none';

function exibeRelatorioGeral(tipo){
	var formulario = document.formulario;
	document.getElementById("tipo_relatorio").value=tipo;
 selectAllOptions(document.getElementById('ungcod'));
	document.getElementById("formulario").submit();

	formulario.target = 'relatorio';
	
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


	
</script>
