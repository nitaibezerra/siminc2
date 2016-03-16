<?

ini_set( "memory_limit", "512M" );
set_time_limit(0);

include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Mais Educação', 'Executa Tramitação' );

$sql_finalizado = "SELECT
						count(*)
					FROM
						pdeescola.meloteimpressao mli
					INNER JOIN 
						pdeescola.memaiseducacao mme ON mme.entid = mli.entid AND mme.memanoreferencia = 2009
					INNER JOIN 
						workflow.documento doc ON doc.docid = mme.docid
					INNER JOIN 
						workflow.estadodocumento est ON est.esdid = doc.esdid AND est.esdid = 34";

$sql_rel_emitido = "SELECT
						count(*)
					FROM
						pdeescola.meloteimpressao mli
					INNER JOIN 
						pdeescola.memaiseducacao mme ON mme.entid = mli.entid AND mme.memanoreferencia = 2009
					INNER JOIN 
						workflow.documento doc ON doc.docid = mme.docid
					INNER JOIN 
						workflow.estadodocumento est ON est.esdid = doc.esdid AND est.esdid = 39";

echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"0\" cellPadding=\"3\" align=\"center\">
		<tr>
			<td align=\"center\">
				<br />
				N° de registros na tabela 'meloteimpressao': ".$db->pegaUm("SELECT count(*) FROM pdeescola.meloteimpressao")."
			</td>
		</tr>
		<tr>
			<td align=\"center\">
				N° de registros na tabela 'meloteimpressao' (finalizado): ".$db->pegaUm($sql_finalizado)."
			</td>
		</tr>
		<tr>
			<td align=\"center\">
				N° de registro na tabela 'meloteimpressao' (relatório emitido): ".$db->pegaUm($sql_rel_emitido)."
			</td>
		</tr>";

$esdidFinalizado = 34;

$sql = "SELECT
			mli.*
		FROM
			pdeescola.meloteimpressao mli
		INNER JOIN 
			pdeescola.memaiseducacao mme ON mme.entid = mli.entid AND mme.memanoreferencia = " . $_SESSION["exercicio"] . "
		INNER JOIN 
			workflow.documento doc ON doc.docid = mme.docid
		INNER JOIN 
			workflow.estadodocumento est ON est.esdid = doc.esdid AND est.esdid = ".$esdidFinalizado."
		ORDER BY
			mli.mlicodigolote";
$dadosLote = $db->carregar($sql);

// O 'aedid' da ação: finalizado -> relatório emitido. (produção)
$aedid = 211;

if($dadosLote) {
	for($i=0; $i<count($dadosLote); $i++) {
		// Recupera o 'docid' da entidade
		$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE entid = ".$dadosLote[$i]["entid"]." AND memanoreferencia = " . $_SESSION["exercicio"];
		$docid = $db->pegaUm($sql);
		
		$docid = (integer) $docid;
		$aedid = (integer) $aedid;
		
		$acao = wf_pegarAcao2( $aedid );
		$esdiddestino = (integer) $acao['esdiddestino'];
		
		// cria log no histórico
		$sqlHistorico = "
			insert into workflow.historicodocumento
			( aedid, docid, usucpf, htddata )
			values ( " . $aedid . ", " . $docid . ", '" . $_SESSION['usucpf'] . "', '".$dadosLote[$i]["mlidataimpressao"]."' )";
		$db->executar( $sqlHistorico );
		
		$sqlDocumento = "
			update workflow.documento
			set esdid = " . $esdiddestino . "
			where docid = " . $docid;
		$db->executar( $sqlDocumento );
	}
	
	//$db->commit();
}

?>
	<tr>
		<td align="center">
			<br /><br />
			<b><font color="red">Tramitação Realizada!</font></b>
			<br /><br />
		</td>
	</tr>
	<tr>
		<td align="center">
			<b>N° de registro na tabela 'meloteimpressao' (finalizado): <?= $db->pegaUm($sql_finalizado) ?></b>
		</td>
	</tr>
	<tr>
		<td align="center">
			<b>N° de registro na tabela 'meloteimpressao' (relatório emitido): <?= $db->pegaUm($sql_rel_emitido) ?></b>
			<br /><br />
		</td>
	</tr>
<?

$loteAraguaina = array(0 => '153223',
					   1 => '153272',
					   2 => '153276',
					   3 => '153280',
					   4 => '153283',
					   5 => '153294',
					   6 => '153295');

for($i=0; $i<count($loteAraguaina); $i++) {
	// Recupera o 'docid' da entidade
	$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE entid = ".$loteAraguaina[$i]." AND memanoreferencia = " . $_SESSION["exercicio"];
	$docid = $db->pegaUm($sql);
	
	$docid = (integer) $docid;
	
	$sqlDocumento = "
		update workflow.documento
		set esdid = 34
		where docid = " . $docid;
	$db->executar( $sqlDocumento );
}
	
//$db->commit();

?>
	<tr>
		<td align="center">
			<br /><br />
			<b><font color="red">Escolas municipais de Araguaína tiveram o estado alterado para 'Finalizado' com sucesso!</font></b>
			<br /><br />
		</td>
	</tr>
<?

$sql = "DELETE FROM pdeescola.meloteimpressao WHERE entid = 214803 AND estuf = 'RJ'";
$db->executar($sql);

// Recupera o 'docid' da entidade
$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE entid = 214803 AND memanoreferencia = " . $_SESSION["exercicio"];
$docid = $db->pegaUm($sql);

$docid = (integer) $docid;

$sqlDocumento = "
	update workflow.documento
	set esdid = 34
	where docid = " . $docid;
$db->executar( $sqlDocumento );

$db->commit();

?>
	<tr>
		<td align="center">
			<br /><br />
			<b><font color="red">A escola 'E.E. NORONHA SANTOS' foi excluída da tabela de Lote e seu estado foi alterado para 'Finalizado'!</font></b>
			<br /><br />
		</td>
	</tr>
</table>