<?

function cabecalhoAtendimento(){
	global $db;

	$ateid = $_SESSION['ateid'];

	if (!$ateid){
		echo "<script>window.location.href=\"atendefnde.php?modulo=principal/listaAtendimento&acao=A\";</script>";
		exit;
	}

	$sql = "SELECT
			 ateassunto as assunto,
			 atedescricao as descricao,
			 to_char(atedatainclusao, 'DD/MM/YYYY') AS dataabertura
			FROM
			 atendefnde.atendimento
			WHERE
			 ateid = {$ateid}";
	$dados = $db->carregar($sql);
	if($dados[0]){
		extract($dados[0]);
		
		for($i=0;$i<count($dados);$i++){
			$sql = "select soldescricao from atendefnde.atendimentosolicitante aso
					inner join atendefnde.solicitante s on s.solid = aso.solid
					where aso.ateid = ". (int)$ateid ;
			$solicitante = $db->carregarColuna($sql);
			if($solicitante){
				$responsavel = implode('; ',$solicitante) . '.';
			}
		}
	}

	

	$cab = "<table align=\"center\" class=\"Tabela\" style='border-bottom:2px solid #000;'>
			 <tbody>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nº do atendimento:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$ateid}</td>
				</tr>
			 	<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Assunto:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$assunto}</td>
				</tr>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Solicitantes:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$responsavel}</td>
				</tr>			 
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Data de Abertura:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dataabertura}</td>
				</tr>								 
			 </tbody>
			</table>";
	return $cab;
}


function cabecalhoSolicitacao(){
	global $db;

	$solid = $_SESSION['solid'];

	if (!$solid){
		echo "<script>window.location.href=\"atendefnde.php?modulo=principal/listaSolicitacao&acao=A\";</script>";
		exit;
	}

	$sql = "SELECT
			 s.ateid,
			 s.solassunto as assunto,
			 s.soldescricao as descricao,
			 u.usunome as responsavel
			FROM
			 atendefnde.solicitacao s
			 left join atendefnde.responsavelsolicitacao r on r.rsoid = s.rsoid
			 inner join seguranca.usuario u on u.usucpf = r.usucpf
			WHERE
			 solid = {$solid}";
	$dados = $db->carregar($sql);
	if($dados[0]){
		extract($dados[0]);
	}

	$cab = "<table align=\"center\" class=\"Tabela\" style='border-bottom:2px solid #000;'>
			 <tbody>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nº do atendimento:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$ateid}</td>
				</tr>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Nº da Solicitação:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$solid}</td>
				</tr>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Assunto:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$assunto}</td>
				</tr>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Responsável:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$responsavel}</td>
				</tr>			 
			 </tbody>
			</table>";
	return $cab;
}


/******************
 * Pega array com perfis
 ******************/
function arrayPerfil(){
	global $db;

	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 44
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
	$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}



function inserirAnexoAtendimento($dados) {

	if($_FILES['arquivo']['error'] == 0) {

		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

		$campos = array("ateid"         => "'".$dados['ateid']."'",
						"usucpf"        => "'".$_SESSION['usucpf']."'",
						"atadescricao"  => "'".$dados['dsc_']."'",
						"atadatainclusao" => "NOW()",
						"atastatus"     => "'A'");

		$file = new FilesSimec("atendimentoanexo", $campos ,"atendefnde");
		$file->setUpload( (($dados['dsc_'])?$dados['dsc_']:NULL), $key = "arquivo" );

		if($dados['redirecionamento']) {

			echo "<script>
					alert('Arquivo anexado com sucesso');
					window.location='".$dados['redirecionamento']."';
				  </script>";

		}

	}
}

function removerAnexoAtendimento($dados) {
	global $db;

	$sql = "UPDATE atendefnde.atendimentoanexo SET atastatus='I' WHERE ataid='".$dados['ataid']."'";
	$db->executar($sql);
	$db->commit();
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("atendimentoanexo", NULL ,"atendefnde");
	$file->excluiArquivoFisico($dados['arqid']);
	
	echo "<script>
			alert('Anexo removido com sucesso');
			window.location='".$_SERVER['HTTP_REFERER']."';
		  </script>";
}

function downloadAnexoAtendimento($dados) {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("atendimentoanexo", NULL ,"atendefnde");
	$file->getDownloadArquivo($dados['arqid']);
}



function inserirAnexoSolicitacao($dados) {

	if($_FILES['arquivo']['error'] == 0) {

		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

		$campos = array("solid"         => "'".$dados['solid']."'",
						"usucpf"        => "'".$_SESSION['usucpf']."'",
						"soadescricao"  => "'".$dados['dsc_']."'",
						"soadatainclusao" => "NOW()",
						"soastatus"     => "'A'");

		$file = new FilesSimec("solicitacaoanexo", $campos ,"atendefnde");
		$file->setUpload( (($dados['dsc_'])?$dados['dsc_']:NULL), $key = "arquivo" );

		if($dados['redirecionamento']) {

			echo "<script>
					alert('Arquivo anexado com sucesso');
					window.location='".$dados['redirecionamento']."';
				  </script>";

		}

	}
}

function removerAnexoSolicitacao($dados) {
	global $db;

	$sql = "UPDATE atendefnde.solicitacaoanexo SET soastatus='I' WHERE soaid='".$dados['soaid']."'";
	$db->executar($sql);
	$db->commit();
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("solicitacaoanexo", NULL ,"atendefnde");
	$file->excluiArquivoFisico($dados['arqid']);
	
	echo "<script>
			alert('Anexo removido com sucesso');
			window.location='".$_SERVER['HTTP_REFERER']."';
		  </script>";
}

function downloadAnexoSolicitacao($dados) {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("solicitacaoanexo", NULL ,"atendefnde");
	$file->getDownloadArquivo($dados['arqid']);
}
?>