<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Perfil dos participantes do PNAIC</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

if(!$_REQUEST['dias']) $_REQUEST['dias'] = "45";

echo '<p> Instrumentos : ';

$sql = "SELECT intid as codigo, intdsc||' ( '||COALESCE(inttextoperguntas,'')||' )' as descricao FROM sispacto2.instrumentostitulo ORDER BY intordem";

$db->monta_combo('intid', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'intid');

echo '</p>';

echo '<p style=font-size:x-small;>A geração dos arquivos podem demorar até 4 minutos para serem gerados!</p>';

echo '<p align="center"><input type="button" value="Ver XLS" onclick="window.location=\'sispacto2.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_instrumentos.inc&intid=\'+document.getElementById(\'intid\').value;"></p>';

if($_REQUEST['intid']) :


	$sql = "SELECT * FROM sispacto2.instrumentostitulo WHERE intid='".$_REQUEST['intid']."'";
	$instrumentostitulo = $db->pegaLinha($sql);
	
	$sql = "SELECT * FROM sispacto2.instrumentosperguntas WHERE intid='".$_REQUEST['intid']."' ORDER BY inpordem";
	$instrumentosperguntas = $db->carregar($sql);
	
	$cabecalho = array("Nº de Identificação (CPF)","Universidade","Rede","Idade","Sexo","Vínculo","Função atual ou similar","Formação (escolaridade)","Área de formação", "Situação formação","Início","Fim","Território de atuação");
	
	if($instrumentosperguntas[0]) {
		foreach($instrumentosperguntas as $ip) {
			$cabecalho[] = $ip['inpdsc'];
			$perguntas[] = $ip['inpid'];
			$sqlcol[] = "(SELECT inodsc FROM sispacto2.instrumentosrespostas ir INNER JOIN sispacto2.instrumentosopcoes io ON ir.inoid = io.inoid WHERE ir.inpid='".$ip['inpid']."' AND ir.iusd = i.iusd) as item".$ip['inpid'];
		}
	}
	
	$sql = "SELECT 
				replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as cpf,
				uu.unisigla||' - '||uu.uninome as universidade,
				CASE WHEN pp.muncod IS NOT NULL THEN 'Municipal'
					 WHEN pp.estuf  IS NOT NULL THEN 'Estadual' END as rede,
				extract(year from age(iusdatanascimento)) as idade,
				i.iussexo,
				tp.tvpdsc,
				fu.funnome,
				es.foedesc,
				cf.cufcursodesc,
				CASE WHEN icf.iufsituacaoformacao='C' THEN 'Completo'
					 WHEN icf.iufsituacaoformacao='I' THEN 'Incompleto'
					 WHEN icf.iufsituacaoformacao='E' THEN 'Em andamento' END as iufsituacaoformacao,
				icf.iufdatainiformacao,
				icf.iufdatafimformacao,
				m.mundescricao||'/'||m.estuf as territorio,
				".implode(",",$sqlcol)."
			FROM sispacto2.identificacaousuario i  
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pe ON pe.pflcod = t.pflcod 
			INNER JOIN sispacto2.universidadecadastro un ON un.uncid = i.uncid 
			INNER JOIN sispacto2.universidade uu ON uu.uniid = un.uniid 
			LEFT JOIN sispacto2.pactoidadecerta pp ON pp.picid = i.picid 
			INNER JOIN public.tipovinculoprofissional tp ON tp.tvpid = i.tvpid 
			INNER JOIN sispacto2.funcao fu ON fu.funid = i.funid 
			INNER JOIN sispacto2.formacaoescolaridade es ON es.foeid = i.foeid 
			INNER JOIN sispacto2.identiusucursoformacao icf ON icf.iusd = i.iusd 
			INNER JOIN sispacto2.cursoformacao cf ON cf.cufid = icf.cufid  
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
			WHERE i.iusd IN(SELECT DISTINCT iusd FROM sispacto2.instrumentosrespostas WHERE inpid IN('".implode("','",$perguntas)."'))";
	
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_contatos_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_instrumentos_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,1000000000,5,'N','100%','');
	exit;
	


endif;
?>