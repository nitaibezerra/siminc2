<?php

function monta_sql(){
	global $db;
	extract($_REQUEST);

	$where = array();
	$whereinst = array();
		
	// universidade
	if( $unicod[0] && $unicod_campo_flag != '' ){
		array_push($where, " u.unicod " . (!$unicod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( ',', $unicod ) . "') ");
		array_push($whereinst, " u.unicod " . (!$unicod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( ',', $unicod ) . "') ");
		
	}
	
	// estado
	if( $estuf[0] && $estuf_campo_flag != '' ){
		array_push($where, "mu.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "')");
		array_push($whereinst, "mu.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "')");
	}
	
	// municipio
	if( $muncod[0] && $muncod_campo_flag != '' ){
		array_push($where, "mu.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "')");
		array_push($whereinst, "mu.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "')");
	}

	// parcela
	if( $fpbid[0] && $fpbid_campo_flag != '' ){
		array_push($where, " f.fpbid " . (!$fpbid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fpbid ) . "') ");
		array_push($whereinst, " f.fpbid " . (!$fpbid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fpbid ) . "') ");
	}
	
	// perfil
	if( $pflcod[0]  && $pflcod_campo_flag != '' ){
		array_push($where, " p.pflcod " . (!$pflcod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pflcod ) . "') ");
		array_push($whereinst, " p.pflcod " . (!$pflcod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pflcod ) . "') ");
	}
	
	// secretaria
	if( $coordid[0]  && $coordid_campo_flag != '' ){
		array_push($where, "( cor.coordid " . (!$coordid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $coordid ) . "') OR cor2.coordid " . (!$coordid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $coordid ) . "') OR cor3.coordid " . (!$coordid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $coordid ) . "') OR cor4.coordid " . (!$coordid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $coordid ) . "') )");
	}
	
	// situacao
	if( $esdid[0]  && $esdid_campo_flag != '' ){
		array_push($where, " es.esdid " . (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ");
		array_push($whereinst, " es.esdid " . (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ");
	}
	
	if($iusnome) {
		array_push($where, " UPPER(public.removeacento(us.iusnome)) ILIKE '%".removeAcentos($iusnome)."%' ");
		array_push($whereinst, " UPPER(public.removeacento(us.iusnome)) ILIKE '%".removeAcentos($iusnome)."%' ");
	}
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_COORDENADOR_INST,$perfis)) {
		$sql = "SELECT DISTINCT unicod FROM sisfor.sisfories WHERE usucpf='".$_SESSION['usucpf']."'";
		$unicods = $db->carregarColuna($sql);
	
		if($unicods) {
			array_push($where, " s.unicod IN('".implode("','",$unicods)."') ");
			array_push($whereinst, " s.unicod IN('".implode("','",$unicods)."') ");
		} else {
			array_push($where, " 1=2 ");
			array_push($whereinst, " 1=2 ");
		}
	
	}
	
	if(in_array(PFL_COORDENADOR_CURSO,$perfis) || in_array(PFL_COORDENADOR_ADJUNTO_IES,$perfis)) {
		
		$sql = "SELECT t.sifid FROM sisfor.identificacaousuario i 
				INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN(".PFL_COORDENADOR_CURSO.",".PFL_COORDENADOR_ADJUNTO_IES.") 
				WHERE i.iuscpf='".$_SESSION['usucpf']."' AND sifid IS NOT NULL";
		
		$sifids = $db->carregarColuna($sql);
		
		array_push($whereinst, " 1=2 ");
		
		if($sifids) {
			array_push($where, " s.sifid IN('".implode("','",$sifids)."') ");
		} else {
			array_push($where, " 1=2 ");
		}

	}
	
	
	
	if(in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis) || in_array(PFL_DIRETOR_MEC,$perfis)) {
		$sql = "SELECT DISTINCT coordid FROM sisfor.usuarioresponsabilidade WHERE pflcod IN(".PFL_EQUIPE_MEC.",".PFL_COORDENADOR_MEC.",".PFL_DIRETOR_MEC.") AND usucpf='".$_SESSION['usucpf']."'";
		$coordids = $db->carregarColuna($sql);
		
		if($coordids) {
			array_push($where, " (cor.coordid in('".implode("','",$coordids)."') OR cor2.coordid in('".implode("','",$coordids)."') OR cor3.coordid in('".implode("','",$coordids)."') OR cor4.coordid in('".implode("','",$coordids)."')) ");
		} else {
			array_push($where, " 1=2 ");
		}
		
	}
	
	$sql = "
		(
			
		SELECT 
		us.iusnome || ' ( ' || replace(to_char(us.iuscpf::numeric, '000:000:000-00'), ':', '.') || ' )' as nome,
		pb.pbovlrpagamento as valor,
		es.esddsc as situacao,
		p.pfldsc as perfil,
		'Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as parcela,
		u.uniabrev||' - '||u.unidsc as universidade,
		COALESCE(mu.mundescricao,'Não identificado') as municipio,
		COALESCE(mu.estuf,'Não identificado') as estado,
		to_char(htddata,'dd/mm/YYYY HH24:MI') as datatramitacao,

		CASE WHEN s.ieoid IS NOT NULL THEN cur.curid  ||' - '|| cur.curdesc 
		     WHEN s.cnvid IS NOT NULL THEN cur2.curid ||' - '|| cur2.curdesc 
		     WHEN s.ocuid IS NOT NULL THEN oc.ocunome 
		     WHEN s.oatid IS NOT NULL THEN oatnome END as curso,

		CASE WHEN s.ieoid IS NOT NULL THEN cor.coordsigla 
		     WHEN s.cnvid IS NOT NULL THEN cor2.coordsigla 
		     WHEN s.ocuid IS NOT NULL THEN cor3.coordsigla 
		     WHEN s.oatid IS NOT NULL THEN cor4.coordsigla END as secretaria,

		CASE WHEN s.ieoid IS NOT NULL THEN CASE WHEN s.sifopcao='1' THEN 'Curso Proposto pelo MEC – Aceito' 
							WHEN s.sifopcao='2' THEN 'Curso Proposto pelo MEC – Rejeitado'
							WHEN s.sifopcao='3' THEN 'Curso Proposto pelo MEC – Repactuado'
							ELSE 'não definido' END
		     WHEN s.cnvid IS NOT NULL THEN 'Curso Proposto pela IES – Do Catálogo' 
		     WHEN s.ocuid IS NOT NULL THEN 'Curso Proposto pela IES – Fora do Catálogo' 
		     WHEN s.oatid IS NOT NULL THEN 'Outras Atividades' END as tipo


			 
		FROM sisfor.pagamentobolsista pb 
		INNER JOIN sisfor.tipoperfil t ON t.tpeid = pb.tpeid 
		INNER JOIN sisfor.sisfor s ON s.sifid = t.sifid  

		LEFT JOIN catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
		LEFT JOIN catalogocurso2014.curso cur on cur.curid = ieo.curid
		LEFT JOIN catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
		LEFT JOIN sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
		LEFT JOIN catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
		LEFT JOIN catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
		LEFT JOIN sisfor.outrocurso oc on oc.ocuid = s.ocuid
		LEFT JOIN catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
		LEFT JOIN sisfor.outraatividade oat on oat.oatid = s.oatid 
		LEFT JOIN catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid


		INNER JOIN public.unidade u ON u.unicod = s.unicod 
		INNER JOIN sisfor.folhapagamento f ON f.fpbid = pb.fpbid 
		INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
		INNER JOIN seguranca.perfil p ON p.pflcod = pb.pflcod 
		INNER JOIN workflow.documento d ON d.docid = pb.docid AND d.tpdid=".WF_TPDID_PAGAMENTOBOLSA." 
		LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid 
		INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
		INNER JOIN sisfor.identificacaousuario us ON us.iusd = pb.iusd 
		LEFT JOIN territorios.municipio mu ON mu.muncod = us.muncodatuacao 
		".(($where)?"WHERE ".implode(" AND ",$where):"")." 
		ORDER BY ".(($agrupador)?implode(",",$agrupador).",":"")."(fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-01')::date,us.iusnome
				
		) UNION ALL (
	
		SELECT 
		us.iusnome || ' ( ' || replace(to_char(us.iuscpf::numeric, '000:000:000-00'), ':', '.') || ' )' as nome,
		pb.pbovlrpagamento as valor,
		es.esddsc as situacao,
		p.pfldsc as perfil,
		'Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as parcela,
		u.uniabrev||' - '||u.unidsc as universidade,
		COALESCE(mu.mundescricao,'Não identificado') as municipio,
		COALESCE(mu.estuf,'Não identificado') as estado,
		to_char(htddata,'dd/mm/YYYY HH24:MI') as datatramitacao,

		'Não se aplica' as curso,

		'Não se aplica' as secretaria,

		'Não se aplica' as tipo


			 
		FROM sisfor.pagamentobolsista pb 
		INNER JOIN sisfor.tipoperfil t ON t.tpeid = pb.tpeid AND t.pflcod='".PFL_COORDENADOR_INST."'
		INNER JOIN sisfor.identificacaousuario us ON us.iusd = pb.iusd 
		INNER JOIN sisfor.sisfories s ON s.usucpf = us.iuscpf  
		INNER JOIN public.unidade u ON u.unicod = s.unicod 
		INNER JOIN sisfor.folhapagamento f ON f.fpbid = pb.fpbid 
		INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
		INNER JOIN seguranca.perfil p ON p.pflcod = pb.pflcod 
		INNER JOIN workflow.documento d ON d.docid = pb.docid AND d.tpdid=".WF_TPDID_PAGAMENTOBOLSA." 
		LEFT JOIN workflow.historicodocumento hst ON hst.hstid = d.hstid 
		INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
		LEFT JOIN territorios.municipio mu ON mu.muncod = us.muncodatuacao 
		".(($whereinst)?"WHERE ".implode(" AND ",$whereinst):"")." 
		ORDER BY ".(($agrupador)?implode(",",$agrupador).",":"")."(fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-01')::date,us.iusnome
	
		)";

	return $sql;
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADOR_CURSO,$perfis) || in_array(PFL_COORDENADOR_ADJUNTO_IES,$perfis)) {
	$_REQUEST['tiporelatorio'] = 'xls';
}


// exibe consulta
if($_REQUEST['tiporelatorio']){
	switch($_REQUEST['tiporelatorio']) {
		case 'xls':
			ob_clean();
			/* configurações */
			ini_set("memory_limit", "2048M");
			set_time_limit(0);
			/* FIM configurações */
			$sql = monta_sql();
			header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
			header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
			header ( "Pragma: no-cache" );
			header ( "Content-type: application/xls; name=rel_coordenador_".date("Ymdhis").".xls");
			header ( "Content-Disposition: attachment; filename=rel_coordenador_".date("Ymdhis").".xls");
			header ( "Content-Description: MID Gera excel" );
			$arCabecalho = array("Nome (CPF)","Valor(R$)","Situação","Perfil","Parcela","Universidade","Município","UF","Data Tramitação","Curso","Tipo Curso");
			$db->monta_lista_tabulado($sql,$arCabecalho,100000000,5,'N','100%','');
			exit;
		case 'html':
			include "relatoriopagamento_resultado.inc";
			exit;
	}
	
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
monta_titulo( "Relatório Pagamento", 'Selecione os filtros e agrupadores desejados' );

?>


<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>


<script type="text/javascript">

	
function exibeRelatorioGeral(tipo){
	
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );
	
	if(tipo != 'xls') {
	
		selectAllOptions( formulario.agrupador );
		// verifica se tem algum agrupador selecionado
		if ( !agrupador.options.length ) {
			alert( 'Escolha ao menos um item para agrupar o resultado.' );
			return false;
		}
	
	}
	
    selectAllOptions(document.getElementById('unicod'));
    selectAllOptions(document.getElementById('estuf'));
    selectAllOptions(document.getElementById('muncod'));
    selectAllOptions(document.getElementById('fpbid'));
    selectAllOptions(document.getElementById('pflcod'));
    selectAllOptions(document.getElementById('coordid'));
    selectAllOptions(document.getElementById('esdid'));
    
	
	formulario.action = 'sisfor.php?modulo=relatorio/relatoriopagamento&acao=A&tiporelatorio='+tipo;
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
<tr>
	<td class="SubTituloDireita" width="15%">Agrupadores:</td>
	<td>
	<?php
	// Início dos agrupadores
	$agrupador = new Agrupador('filtro','');
	
	// Dados padrão de destino (nulo)
	$destino = array();
	$destino = $agrupadorAux;
	
	$origem = array(
		'universidade' => array(
			'codigo'    => 'universidade',
			'descricao' => 'Universidade'
		),
		'curso' => array(
				'codigo'    => 'curso',
				'descricao' => 'Curso'
		),
		'secretaria' => array(
				'codigo'    => 'secretaria',
				'descricao' => 'Secretaria'
		),
		'tipo' => array(
				'codigo'    => 'tipo',
				'descricao' => 'Tipo Curso'
		),
		'perfil' => array(
			'codigo'    => 'perfil',
			'descricao' => 'Perfil'
		),
		'municipio' => array(
			'codigo'    => 'municipio',
			'descricao' => 'Município'
		),
		'estado' => array(
			'codigo'    => 'estado',
			'descricao' => 'UF'
		)
		
		
	);
					
	// exibe agrupador
	$agrupador->setOrigem( 'naoAgrupador', null, $origem );
	$agrupador->setDestino( 'agrupador', null, $destino );
	$agrupador->exibir();
	?>
	</td>
</tr>
<?php

$perfis = pegaPerfilGeral();

	
$stSql = "SELECT
				u.unicod AS codigo,
				u.uniabrev||' - '||u.unidsc AS descricao
			FROM 
				sisfor.sisfories s 
			INNER JOIN public.unidade u ON u.unicod = s.unicod 
			ORDER BY
				2 ";
mostrarComboPopup( 'Universidades:', 'unicod',  $stSql, '', 'Selecione a(s) Universidade(s)' );				

echo '<tr>';
echo '<td class="SubTituloDireita">Nome do bolsista:</td>';
echo '<td>'.campo_texto('iusnome', "N", "S", "Nome", 45, 60, "", "", '', '', 0, 'id="iusnome"' ).'</td>';
echo '</tr>';


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

$stSql = "SELECT f.fpbid as codigo, 'Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
			FROM sisfor.folhapagamento f 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE f.fpbstatus='A' ORDER BY fpbanoreferencia::text||m.mescod||'01'";

mostrarComboPopup( 'Parcelas:', 'fpbid',  $stSql, '', 'Selecione a(s) parcela(s)' );


$stSql = "SELECT p.pflcod as codigo, p.pfldsc as descricao 
			FROM seguranca.perfil p 
			INNER JOIN sisfor.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE p.pflstatus='A' AND p.sisid=".SIS_SISFOR." ORDER BY 2";

mostrarComboPopup( 'Perfis:', 'pflcod',  $stSql, '', 'Selecione o(s) perfil(s)' );

$stSql = "SELECT coordid as codigo, coordsigla as descricao FROM catalogocurso2014.coordenacao WHERE coorano='".$_SESSION['exercicio']."' ORDER BY coordid";

mostrarComboPopup( 'Secretaria:', 'coordid',  $stSql, '', 'Selecione a(s) secretaria(s)' );


$stSql = "SELECT e.esdid as codigo, e.esddsc as descricao 
			FROM workflow.estadodocumento e 
			WHERE e.esdstatus='A' AND e.tpdid=".WF_TPDID_PAGAMENTOBOLSA." ORDER BY 2";

mostrarComboPopup( 'Situações do pagamento:', 'esdid',  $stSql, '', 'Selecione a(s) situaçao(ões)' );

?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				 <input type="button" value="Visualizar HTML" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/>
				 <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>

</form>