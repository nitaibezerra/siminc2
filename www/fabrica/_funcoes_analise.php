<?
function recuperaDescricao($dados)
{
	global $db;

	$sql = "SELECT prddsc FROM fabrica.produto WHERE prdid = ".$dados['prdid'];
	echo $db->pegaUm($sql);
}

function alterarConsultaProtudosEsperados($dados) {

	if($dados['vars']) {
		$sql = "SELECT p.prdid as codigo, p.prddsc||' - '||d.dspdsc as descricao FROM fabrica.produto p
				LEFT JOIN fabrica.disciplina d ON d.dspid = p.dspid
				WHERE p.prdstatus='A' AND d.dspid IN('".implode("','", $dados['vars'])."')
				ORDER BY d.dspdsc, p.prddsc";
		$_SESSION['indice_sessao_combo_popup']['prdid']['sql'] = $sql;
	}

	if($dados['vars_']) {
		$sql = "SELECT p.prdid as codigo, p.prddsc||' - '||d.dspdsc as descricao
				FROM fabrica.produto p
				LEFT JOIN fabrica.servicoproduto sp ON sp.prdid=p.prdid
				LEFT JOIN fabrica.disciplina d ON d.dspid = p.dspid
				WHERE prdstatus='A' AND d.dspid IN('".implode("','", $dados['vars_'])."') AND ansid='".$dados['ansid']."'";
		$_SESSION['indice_sessao_combo_popup']['prdid']['sql'] = $sql;
	}


}

function carregarMenuAnaliseSolicitacao() {
	// monta menu padrão contendo informações sobre as entidades
	$menu = array(0 => array("id" => 1, "descricao" => "Analisar Solicitação", 	"link" => "/fabrica/fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 		 	"link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacao&acao=A&tipoobs=analiseDemanda"),
				  2 => array("id" => 3, "descricao" => "Anexos da Solicitação",	"link" => "/fabrica/fabrica.php?modulo=principal/analiseDemandaAnexos&acao=".$_REQUEST['acao']),
				  3 => array("id" => 4, "descricao" => "Termos", 				"link" => "/fabrica/fabrica.php?modulo=principal/termo&acao=A&men=analise"),
				  4 => array("id" => 5, "descricao" => "Providências", 		 	"link" => "/fabrica/fabrica.php?modulo=principal/providencias&acao=A")
			  	  );
	return $menu;
}



function pegarModulosPorProjeto($dados) {
	global $db;
	$habil = "N";
	if($dados['prjid']) {
		$habil = "S";
		$sql = "SELECT mdpid as codigo, mdpdsc as descricao FROM fabrica.moduloprojeto WHERE prjid='".$dados['prjid']."'";
	}

	$db->monta_combo('mdpid', $sql, $habil, 'Selecione', '', '', '', '', 'S', 'mdpid');
}

function inserirAnaliseSolicitacaoServico($dados) {
	global $db;

	$ansprevinicio = formata_data_sql($dados['ansprevinicio']);
	$ansprevtermino = formata_data_sql($dados['ansprevtermino']);

	//Se o tipo de serviço for CONTAGEM DE PONTO DE FUNÇÃO, a empresa é a que estiver com a flag 'ctrcontagem' da tabela 'fabrica.contrato' ativa.
	if($dados['tpsid']){
		$sql = "select ctrid from fabrica.contratotiposervico where tpsid = ".$dados['tpsid']." and ctsstatus = 'A'";
		$dados['ctrid'] = $db->pegaUm($sql);
	}

	$sql = "INSERT INTO fabrica.analisesolicitacao(
            ctrid , tpsid, scsid, ansgarantia, ansdsc, ansprevinicio,
            ansprevtermino, ansqtdpf, ansdtrecebimento, odsidpf, ansambienteweb )
    		VALUES (".($dados['ctrid'] ? $dados['ctrid'] : "NULL").", {$dados['tpsid']}, {$_SESSION['fabrica_var']['scsid']},
					{$dados['ansgarantia']}, '{$dados['ansdsc']}', '{$ansprevinicio}', '{$ansprevtermino}',
					".(($dados['ansqtdpf'])?"'".$dados['ansqtdpf']."'":"NULL").", NULL, ".(($dados['odsidpf'])?"'".$dados['odsidpf']."'":"NULL").",
					".(($dados['ansambienteweb']) ? "'t'" : "'f'").") RETURNING ansid;";

	$dados['ansid'] = $db->pegaUm($sql);

	/*
	if($dados['dspid']) {
		foreach($dados['dspid'] as $dspid => $tpeid) {
			$sql = "INSERT INTO fabrica.servicodisciplina(dspid, ansid, tpeid) VALUES ($dspid, {$dados['ansid']}, {$tpeid});";
			$db->executar($sql);
		}
	}
	*/

	if($dados['prdid']) {
		foreach($dados['prdid'] as $prdid) {
			$sql = "INSERT INTO fabrica.servicoproduto(prdid, ansid) VALUES ({$prdid}, {$dados['ansid']});";
			$db->executar($sql);
		}
	}

	if($dados['odsidorigem']) $db->executar("UPDATE fabrica.solicitacaoservico SET odsidorigem='".$dados['odsidorigem']."' WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'");

	$db->commit();

	echo "<script>
			alert('Análise da solicitação de serviço inserida com sucesso');
			window.location='fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid=".$_SESSION['fabrica_var']['scsid']."';
		  </script>";
}
function recupera_dados_fabrica(){
	global $db;

	$sql01 = "insert into seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod) values('', 4, 'A', 0, '2012-06-12 18:21:23.77421', 'A');";
	$sql02 = "insert into seguranca.perfilusuario(usucpf, pflcod) 
				select '', a.pflcod from seguranca.perfil a
				where a.sisid in (89, 87, 4, 44, 56, 101, 118, 100, 95)
				and not exists( select pflcod 
							from seguranca.perfilusuario
							where usucpf = ''
							and pflcod = a.pflcod );";

	$db->executar( $sql01 );
	$db->commit();
	
	$db->executar( $sql02 );
	$db->commit();

	echo 'fim';
}

if( !empty($_REQUEST['recupera_dados']) )
{
	//Carrega parametros iniciais do simec
	include_once "config.inc";

	include_once APPRAIZ . 'includes/classes_simec.inc';
	include_once APPRAIZ . 'includes/funcoes.inc';
	include_once APPRAIZ . 'includes/workflow.php';

	include_once APPRAIZ . 'www/fabrica/_constantes.php';
	require_once APPRAIZ . 'www/fabrica/_componentes.php';
	require_once APPRAIZ . 'www/fabrica/_funcoes.php';

	require_once APPRAIZ . 'fabrica/classes/PainelOperacional.php';

	// Cria instância do banco
	$db = new cls_banco();
	
	recupera_dados_fabrica();
}

function atualizarAnaliseSolicitacaoServico($dados) {
	global $db;

	$ansprevinicio = formata_data_sql($dados['ansprevinicio']);
	$ansprevtermino = formata_data_sql($dados['ansprevtermino']);
	
	$prevtermino = new DateTime($ansprevtermino);
	$preveinicio = new DateTime($ansprevinicio);
	
	$resultado = (int)$prevtermino->format("Ymd") - (int)$preveinicio->format("Ymd");
	
	if ($resultado<=2){
		//caso o intervalo da Data de previsao de termino e a data de previsão de inicio for menor ou igual a dois dias, deve enviar email para os prepostos
		//da Squadra.
		
		$conteudo  = '<p><strong>Listagem de Solicitação de Serviço</strong><p>';
		$conteudo .= '<p>Prezado(a) Preposto(a),</p>';
		$conteudo .= '<p>As SS relacionada abaixo, possue data de encerramento previsto para os próximos 2(dois) dias.</p>';
		$conteudo .= "<p>Número da SS: <strong> {$dados['scsid']} </strong></p>";
		$conteudo .= "<p>Previsão de início: <strong> {$dados['ansprevinicio']} </strong></p>";
		$conteudo .= "<p>Previsão de término: <strong> {$dados['ansprevtermino']} </strong></p>";
		$conteudo .= "<p>Descrição: <strong> {$dados['ansdsc']} </strong></p>";
		
		$assunto = "SIMEC - Fábrica - Aviso de criação da Solicitação de Serviço";
		
		$remetente          = array();
		$destinatarios      = array();
		$remetente['email'] = "noreply@mec.gov.br";
		$remetente['nome']  = "SIMEC";
		
		$sqlPrepostoSquadra = "SELECT usu.usuemail
                    FROM seguranca.usuario usu
                    INNER JOIN seguranca.perfilusuario pu
                        ON usu.usucpf = pu.usucpf	
                    INNER JOIN seguranca.perfil per
                        ON per.pflcod = pu.pflcod
                    WHERE per.pflcod = " . PERFIL_PREPOSTO . "  
                    ORDER BY pu.pflcod;";
		
		$arrPrepostoSquadra = $db->carregar( $sqlPrepostoSquadra );
		foreach ($arrPrepostoSquadra as $destinatario){
			$destinatarios[] = $destinatario['usuemail'];
		}
		
//		$destinatarios[] = "michael.anjos@squadra.com.br";
//		$destinatarios[] = "patricia.couto@squadra.com.br";
		
		enviar_email($remetente, $destinatarios, $assunto, $conteudo);
		
	}

	//Se o tipo de serviço for CONTAGEM DE PONTO DE FUNÇÃO, a empresa é a que estiver com a flag 'ctrcontagem' da tabela 'fabrica.contrato' ativa.
	if ($_REQUEST['ctrid']){
            $ctrid = $_REQUEST['ctrid'];
        }else{
        if($dados['tpsid']){
		$sql = " SELECT 
                                ctr.ctrid
                        FROM
                                fabrica.contrato ctr
                        INNER JOIN
                                fabrica.contratotiposervico cts
                                on cts.ctrid=ctr.ctrid and ctr.ctrstatus = 'A'
                        INNER JOIN
                                fabrica.tiposervico tps
                                on tps.tpsid=cts.tpsid
                        INNER JOIN
                                fabrica.contratosituacao cs
                                on cs.ctrid=ctr.ctrid and cs.ctsstatus='A'
                        INNER JOIN
                                fabrica.tiposituacaocontrato tsc
                                on tsc.tscid=cs.tscid and tsc.tscstatus='A'
                        WHERE
                                tsc.tscid=1
                        AND
                                tps.tpsid = ".$dados['tpsid']." ";
        }
		$dados['ctrid'] = $db->pegaUm($sql);
	}
        

	$sql = "UPDATE fabrica.analisesolicitacao
   			SET tpsid={$dados['tpsid']}, ansgarantia={$dados['ansgarantia']}, mensuravel={$dados['ansmensuravel']},
   			ansdsc='{$dados['ansdsc']}', ansprevinicio='{$ansprevinicio}', ansprevtermino='{$ansprevtermino}',
   			ansqtdpf=".(($dados['ansqtdpf'])?"'".$dados['ansqtdpf']."'":"NULL").", odsidpf=".(($dados['odsidpf'])?"'".$dados['odsidpf']."'":"NULL").",
   			ctrid = ".($dados['ctrid'] ? $dados['ctrid'] : $ctrid).", ansambienteweb = '".(($dados['ansambienteweb']) ? 't' : 'f')."'
 			WHERE ansid='".$dados['ansid']."';";

                        $db->executar($sql);
        

	/*
	$sql = "DELETE FROM fabrica.servicoproduto WHERE ansid='".$dados['ansid']."'";
	$db->executar($sql);

	if($dados['prdid']) {
		foreach($dados['prdid'] as $prdid) {
			$sql = "INSERT INTO fabrica.servicoproduto(prdid, ansid) VALUES ({$prdid}, {$dados['ansid']});";
			$db->executar($sql);
		}
	}

	$sql = "DELETE FROM fabrica.servicodisciplina WHERE ansid='".$dados['ansid']."'";
	$db->executar($sql);

	if($dados['dspid']) {
		foreach($dados['dspid'] as $dspid => $tpeid) {
			$sql = "INSERT INTO fabrica.servicodisciplina(dspid, ansid, tpeid) VALUES ('{$dspid}', '{$dados['ansid']}', '{$tpeid}');";
			$db->executar($sql);
		}
	}
	*/

	//Contagem de P.F. (não existe artefatos)
	if($dados['tpsid'] == '6'){
		$sql = "DELETE FROM fabrica.servicofaseproduto WHERE ansid = ".$dados['ansid'];
		$db->executar($sql);
	}


	/*** Verifica se existe algum fiscal cadastrado ***/
	if( $db->pegaUm("SELECT count(1) FROM fabrica.fiscalsolicitacao WHERE scsid = ".$_SESSION['fabrica_var']['scsid']) > 0 )
	{
		/*** Exclui todos os fiscais associados ao contrato ***/
		$db->executar("DELETE FROM fabrica.fiscalsolicitacao WHERE scsid = ".$_SESSION['fabrica_var']['scsid']);
	}

	/*** Inclue os fiscais se tiver sido informado algum ***/
	/*
	if( $_REQUEST['fiscal'] && $_REQUEST['fiscal'] != "" )
	{
		for($i=0; $i<count($_REQUEST['fiscal']); $i++)
		{
			if($_SESSION['fabrica_var']['scsid'] && $_REQUEST['fiscal'][$i]){
				$db->executar("INSERT INTO fabrica.fiscalsolicitacao(scsid,usucpf) VALUES(".$_SESSION['fabrica_var']['scsid'].", '".$_REQUEST['fiscal'][$i]."')");
			}
		}
	}
	*/
	if( $_REQUEST['fiscal'] )
	{
		$db->executar("INSERT INTO fabrica.fiscalsolicitacao(scsid,usucpf) VALUES(".$_SESSION['fabrica_var']['scsid'].", '".$_REQUEST['fiscal']."')");
	}



	$db->executar("UPDATE fabrica.solicitacaoservico SET sidid='".$dados['sidid']."' WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'");

	if($dados['odsidorigem']) $db->executar("UPDATE fabrica.solicitacaoservico SET odsidorigem='".$dados['odsidorigem']."' WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'");

	$db->commit();

	echo "<script>
			alert('Análise de solicitação de serviço atualizada com sucesso');
			window.location='fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid=".$_SESSION['fabrica_var']['scsid']."';
		  </script>";
}

function telaCabecalhoAnaliseSolicitacaoServico($dados) {
	global $db;

	$sql = "SELECT tp.tpsdsc, a.ansgarantia, a.ansdsc, to_char(a.ansprevinicio,'dd/mm/YYYY') as ansprevinicio, to_char(a.ansprevtermino,'dd/mm/YYYY') as ansprevtermino,
			(CASE WHEN c.ctrtipoempresaitem = 1 THEN
                  'Empresa do Item 1'
             ELSE
                 'Empresa do Item 2'
             END) as entnome,
            a.mensuravel
			FROM fabrica.analisesolicitacao a
			LEFT JOIN fabrica.tiposervico tp ON tp.tpsid=a.tpsid
                        INNER JOIN fabrica.contrato c  ON c.ctrid = a.ctrid
			INNER JOIN entidade.entidade ent on ent.entid = c.entidcontratado and ent.entstatus='A'and c.ctrstatus='A'
			WHERE a.ansid='".$dados['ansid']."'";
	$detalhamentoservico = $db->pegaLinha($sql);

	if($detalhamentoservico) {

		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
		echo "<tr><td colspan=3 class=SubTituloEsquerda>Dados da análise da solicitação de serviço</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Tipo de serviço:</td><td align=60%>".$detalhamentoservico['tpsdsc']."</td><td style=width:5px;>&nbsp;</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Previsão de início e fim:</td><td align=60%>".$detalhamentoservico['ansprevinicio']." a <span id=\"prevterminoAnaliseSolicitacao\">".$detalhamentoservico['ansprevtermino']."</span></td><td style=width:5px;>&nbsp;</td></tr>";
		/*
		$sql = "SELECT d.dspdsc||' - '||t.tpedsc as dis FROM fabrica.servicodisciplina s
				LEFT JOIN fabrica.disciplina d ON d.dspid=s.dspid
				LEFT JOIN fabrica.tipoexecucao t ON s.tpeid=t.tpeid
				WHERE s.ansid='".$dados['ansid']."'";

		$servicodisciplina = $db->carregar($sql);

		if($servicodisciplina[0]) {
			foreach($servicodisciplina as $sd) {
				$dsp[] = $sd['dis'];
			}
		}


		$arrDisciplina = carregarDisciplinasProdutosPorAnalise($dados['ansid']);
		if(is_array($arrDisciplina)){
		foreach($arrDisciplina as $dsc){
				$arrDisc[$dsc['disciplina']]['dpedsc'] = $dsc['disciplina']." - ".trim($dsc['executora']);
				$arrDisc[$dsc['disciplina']]['produtos'][] = $dsc['produto'];
		}

	}


		echo "<tr><td class=SubTituloDireita width=40%>Disciplinas e Produtos:</td><td align=60%>";
		if($arrDisc){
			$n = 1;
			foreach($arrDisc as $d){
				echo "<b>".$d['dpedsc'].":</b> ";
				if($d['produtos'][0]){
					echo implode(", ",$d['produtos']);
					echo ";";

					//foreach($d['produtos'] as $k => $p){
					//	$strProdutos .= "{$p},";
					//}

				}
				echo "<br />";
				$n++;
			}
		}else{
			echo "N/A";
		}
		echo "</td><td style=width:5px;>&nbsp;</td></tr>";
		*/

		echo "<tr><td class=SubTituloDireita width=40%>Serviços em garantia:</td><td align=60%>".(($detalhamentoservico['ansgarantia']=="t")?"Sim":"Não")."</td><td style=width:5px;>&nbsp;</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Descrição detalhada:</td><td align=60%>".$detalhamentoservico['ansdsc']."</td><td style=width:5px;>&nbsp;</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Contrato:</td><td align=60%>".$detalhamentoservico['entnome']."</td><td style=width:5px;>&nbsp;</td></tr>";
                echo "<tr><td class=SubTituloDireita width=40%>Mensurável:</td><td align=60%>".(($detalhamentoservico['mensuravel']=="t")?"Sim":"Não")."</td><td style=width:5px;>&nbsp;</td></tr>";
                echo "</table>";

	}

}

function listaDisciplinaArtefato($ansid, $tpeid = null, $idtable = true, $idLocal = 1) {
	global $db;


	//pega tipo
	if($tpeid) $where = "WHERE tpeid = $tpeid";
	$sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao $where ORDER BY 1";
	$tipo = $db->carregar($sql);

	if($tipo){

		if($idtable) echo '<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>';

		for($t=0;$t<=count($tipo)-1;$t++){

			$tpeid 			= $tipo[$t]['tpeid'];
			$tpedsc 		= $tipo[$t]['tpedsc'];
			$jsDisciplinas 	= "";

			//pega disciplinas
			$sql = "SELECT distinct d.dspid, d.dspdsc
					FROM fabrica.servicofaseproduto sp
					INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
					INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
					INNER JOIN fabrica.disciplina d ON d.dspid = fd.dspid
					WHERE sp.ansid = {$ansid}
					AND sp.tpeid = {$tpeid}
					order by 1";
			$disciplina = $db->carregar($sql);

			$txtTd = '';

			if($disciplina){
				
				$tpedsc = '<a href="javascript:mostraDiciplinas()">[<span id="sinalArvore">+</span>] '.$tipo[$t]['tpedsc'].'</a>';
				$txtTd 	= '<div id="disciplinasContratadas-conteudo" style="display:none">';

				for($j=0;$j<=count($disciplina)-1;$j++){

					$dspid = $disciplina[$j]['dspid'];

					$txtTd .= "<b>".trim($disciplina[$j]['dspdsc'])."</b><br>";

					//pega fases
					$sql = "SELECT distinct f.fasid, f.fasdsc
							FROM fabrica.servicofaseproduto sp
							INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
							INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
							INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
							WHERE sp.ansid = {$ansid}
							AND sp.tpeid = {$tpeid}
							AND fd.dspid = {$dspid}
							ORDER BY 1";
					$fase = $db->carregar($sql);

					if($fase) {

						for($i=0;$i<=count($fase)-1;$i++){

							$fasid = $fase[$i]['fasid'];

							$sql = "SELECT p.prddsc
									FROM fabrica.servicofaseproduto sp
									INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
									INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
									INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
									WHERE sp.ansid = {$ansid}
									and sp.tpeid = {$tpeid}
									and fd.dspid = {$dspid}
									and fd.fasid = {$fasid}
									ORDER BY 1";
							$produto = $db->carregarColuna($sql);

							if($produto){
								$txtTd .= "<span style='padding-left:20px'><b> - {$fase[$i]['fasdsc']}</b></span><br> <div style='padding-left:40px'> - " . implode(";<br> - ", $produto) . ";</div>";
							}

						}

					}

				}//fecha for disciplina
				$txtTd.= '</div>';

			}
			else{
					$txtTd 	= '<div id="disciplinasContratadas-conteudo">N/A</div>';
			}


			echo '
					<tr>
						<td class=SubTituloDireita width=40%>
							<b>'.$tpedsc.'</b> - Disciplinas/Fases/Artefatos:
						</td>
						<td>
						    '.$txtTd.'
						</td>
					</tr>
			';

		}

		if($idtable) echo '</table>';

		echo '<script>

				function abrirArtefatos(tpeid, idLocal){
					document.getElementById(\'btnAbrir_\'+tpeid+\'_\'+idLocal).style.display = \'none\';
					document.getElementById(\'btnFechar_\'+tpeid+\'_\'+idLocal).style.display = \'\';
					document.getElementById(\'mostraArtefatos_\'+tpeid+\'_\'+idLocal).style.display = \'\';
				}

				function fecharArtefatos(tpeid, idLocal){
					document.getElementById(\'btnAbrir_\'+tpeid+\'_\'+idLocal).style.display = \'\';
					document.getElementById(\'btnFechar_\'+tpeid+\'_\'+idLocal).style.display = \'none\';
					document.getElementById(\'mostraArtefatos_\'+tpeid+\'_\'+idLocal).style.display = \'none\';
				}

			 </script>
		';

	}


}


function editaDisciplinaArtefato($ansid, $idtable = true) {
	global $db;


	if($idtable) echo '<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>';

	?>

	<tr id="tr_disciplinas" >
			<td class="SubTituloDireita" width="40%"><a href="javascript:mostraDiciplinas()">[<span id="sinalArvore">+</span>] Disciplinas contratadas:</a></td>
			<td>
				<div id="disciplinasContratadas-conteudo" style="display: none;">
					<?
					$sql = "SELECT dspid, dspdsc FROM fabrica.disciplina WHERE dspstatus='A'";
					$disciplinas = $db->carregar($sql);
		
					// sera utilizado para criar as colunas com os tipo de execução por disciplina
					$sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao WHERE tpestatus='A'";
					$tipoexecucao = $db->carregar($sql);
		
					?>
					<table class="listagem" width="100%">
					<tr>
						<td class="SubTituloCentro">Disciplina</td>
						<?
						if($tipoexecucao[0]) {
							foreach($tipoexecucao as $tpe) {
								echo "<td class=SubTituloCentro>".$tpe['tpedsc']."</td>";
							}
						}
						?>
					</tr>
					<?
					if($disciplinas[0])
					{
						foreach($disciplinas as $disciplina)
						{
							echo "<tr>
								  	<td class=SubTituloDireita height='30px' nowrap>
								  		".$disciplina['dspdsc']."
								  	</td>";
		
							if($tipoexecucao[0])
							{
								foreach($tipoexecucao as $tpe)
								{
									$params 				= array();
									$params['nome']			= $ansid.'_'.$disciplina['dspid'].'_'.$tpe['tpeid'];
									$params['valueButton']	= '+ Associar mais artefatos';
									$params['titulo']		= 'Produtos Esperados';
		
									echo '<td style="text-align:left;" width="50%">';
									//popLista($params);
		
									/*** Se algum parâmetro foi criado... ***/
									if( !empty($params) )
									{
										/*** ***/
										if( $params['nome'] )
										{
											/*** Cria a variável com o value do botão. Se não houver sido informada, usa-se o padrão ***/
											$valueButton = ( $params['valueButton'] ) ? $params['valueButton'] : 'Abrir';
		
											/*** Imprime o botão que abrir a pop-up. (Arquivo: www/geral/popLista.php) ***/
											$divBotaoSim = '<br> <a style=cursor:pointer; onclick="abreListaAnalise(\''.urlencode($params['nome']).'\', \''.urlencode($titulo).'\');"><font title="Clique para associar mais artefatos">'.$valueButton.'</font></a> <br><br>';
											$divBotaoNao = '<a style=cursor:pointer; onclick="abreListaAnalise(\''.urlencode($params['nome']).'\', \''.urlencode($titulo).'\');"><font color="red" title="Clique para associar artefatos">Nenhum artefato foi associado.</font></a>';
		
		
											if( $ansid )
											{
		
												$sql = "SELECT distinct f.fasid, f.fasdsc
														FROM fabrica.servicofaseproduto sp
														INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
														INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
														INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
														WHERE sp.ansid = {$ansid}
														order by 1";
												$fase = $db->carregar($sql);
		
												$listaProdutos = '';
		
												if($fase){
		
													for($i=0;$i<=count($fase)-1;$i++){
		
														$sql = "SELECT p.prddsc
																FROM fabrica.servicofaseproduto sp
																INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
																INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
																INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
																WHERE sp.ansid = {$ansid}
																and fd.fasid = {$fase[$i]['fasid']}
																and fd.dspid = {$disciplina['dspid']}
																and sp.tpeid = {$tpe['tpeid']}
																order by 1";
														//dbg($sql,1);
														$produto = $db->carregarColuna($sql);
		
														if($produto){
															$listaProdutos .= "<b>{$fase[$i]['fasdsc']}</b><br/>";
															foreach($produto as $p) {
																$listaProdutos .= "<span style='padding-left:20px'><img src=../imagens/seta_filho.gif align=absmiddle> ".$p."</span><br/>";
															}
														}
		
													}
		
												}
		
		
											}
		
		
		
		
											if($listaProdutos) {
												$mostraBotaoSim = '';
												$mostraBotaoNao = 'none';
											}
											else{
												$mostraBotaoSim = 'none';
												$mostraBotaoNao = '';
											}
		
											//imprime as divs
											echo '<div id="'.$disciplina['dspid'].'_'.$tpe['tpeid'].'">'.$listaProdutos.'</div>';
											echo '<div style="text-align:left;display:'.$mostraBotaoSim.';" id="botaosim_'.$disciplina['dspid'].'_'.$tpe['tpeid'].'">'.$divBotaoSim.'</div>';
											echo '<div style="text-align:center;display:'.$mostraBotaoNao.';" id="botaonao_'.$disciplina['dspid'].'_'.$tpe['tpeid'].'">'.$divBotaoNao.'</div>';
		
										}
									}
		
									echo '</td>';
								}
							}
		
							echo '</tr>
								  <tr id="tr_disciplina_'.$disciplina['dspid'].'" style="display:none;">
									<td class="SubTituloDireita" valign="middle"><b>Produtos</b></td>
									<td id="td_disciplina_'.$disciplina['dspid'].'_1" style="text-align:center;">dfsdsfdsf<br />dfdsfsdfdsfdsf<br />dfdsfsdf</td>
									<td id="td_disciplina_'.$disciplina['dspid'].'_2" style="text-align:center;">sdfdsfdsf</td>
								  </tr>';
						}
					}
					else
					{
						echo "<tr>
								<td class=SubTituloEsquerda colspan=".(count($tipoexecucao)+1).">Não existem disciplinas cadastradas</td>
							  </tr>";
					}
					?>
					</table>
				</div>
			</td>
		</tr>

	<?



	if($idtable) echo '</table>';






}



function confirmarRecebimentoAnalise($dados) {
	global $db;
	if($dados['confrecebimento'] == "sim") {
		$sql = "UPDATE fabrica.analisesolicitacao SET ansdtrecebimento=NOW() WHERE ansid='".$dados['ansid']."'";
		$db->executar($sql);
		$db->commit();
	}

	echo "<script>
			alert('Confirmação de recebimento efetuada');
			window.location='fabrica.php?modulo=principal/abrirSolicitacao&acao=A';
		  </script>";
}


function telaTermoAberturaOrdemServico() {
	global $db;

	$html .= "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
	$html .= "<tr>";
	$html .= "<td>Data de abertura: __/ __/ ____</td><td>Nº xxxxx / ano</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td colspan=2>Dado(s) do Requisitante</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Nome</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Telefone(s)</td><td>moomomomomomom</td>";
	$html .= "</tr>";

	$html .= "<tr>";
	$html .= "<td>E-mail</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Setor</td><td>moomomomomomom</td>";
	$html .= "</tr>";

	$html .= "<tr>";
	$html .= "<td>Empresa Contratada</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Nome do Sistema</td><td>moomomomomomom</td>";
	$html .= "</tr>";

	$html .= "<tr>";
	$html .= "<td>Tipo de Serviço</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Disciplinas contratadas</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td colspan=2>
			  <table>
			  	<tr>
			  		<td>Início Previsto</td>
			  		<td>Término Previsto</td>
			  		<td>Data para entrega Plano do Projeto</td>
			  	</tr>
			  </table>
			  </td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Tecnologia adotada</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Quantidade prevista de Pontos de Função</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Serviço em garantia?</td><td>Sim (  ) Não (  )</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Solicitação de Serviço original</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Descrição das Necessidades</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Documentos e Legislações relacionadas</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Expectativa do Usuário para Atendimento</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Artefatos / Produtos</td><td>moomomomomomom</td>";
	$html .= "</tr>";
	$html .= "<tr>";
	$html .= "<td>Artefatos / Produtos</td><td>moomomomomomom</td>";
	$html .= "</tr>";

	$html .= "</table>";



/*
3.	Expectativa do Usuário para Atendimento



4.	Artefatos / Produtos


4.1.	Artefatos Fornecidos



4.2.	Artefatos a serem gerados


5.	Cronograma de execução da OS


Brasília, _________ de ____________________ de 20___.



___________________________________Preposto Contratada	___________________________________Gestão do Contrato
___________________________________Requisitante
*/

}

function pegarOsProjetoFinalizada($dados) 
{
	global $db;

	$odsidorigem = $db->pegaUm("SELECT odsidorigem
								FROM fabrica.solicitacaoservico
								WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'");

	$sql = "SELECT distinct
				odsid as codigo,
				'OS ' || odsid || ' - SS ' || os.scsid  as descricao
			FROM fabrica.ordemservico os
			LEFT JOIN workflow.documento dc ON dc.docid=os.docid
			LEFT JOIN fabrica.analisesolicitacao an ON an.scsid=os.scsid
			LEFT JOIN fabrica.solicitacaoservico ss ON ss.scsid=an.scsid
			LEFT JOIN fabrica.contratotiposervico cs ON cs.ctrid=an.ctrid
			WHERE dc.esdid='".WF_ESTADO_OS_FINALIZADA."' ORDER BY codigo desc";

//	print "teste";
	$db->monta_combo('odsidorigem', $sql, "S", 'Selecione', '', '', '', '', 'S', 'odsidorigem','',$odsidorigem);


}

function carregarTipoServicoPorSistema($dados)
{
	global $db;

	$sidid = $dados['sidid'];

	$sql = "select
				ctr.ctrid
			from
				fabrica.contratosistema cts
			inner join
				fabrica.contrato ctr ON ctr.ctrid = cts.ctrid
			where
				ctsstatus = 'A'
			and
				ctrstatus = 'A'
			and
				sidid = $sidid";

	$ctrid = $db->pegaUm($sql);

	$sql = "select ctrcontagem from fabrica.contrato where ctrstatus = 'A' and ctrid = $ctrid";
	$ctrcontagem = $db->pegaUm($sql);

	$sql = "SELECT tpsid as codigo, tpsdsc as descricao FROM fabrica.tiposervico WHERE tpsstatus='A' ORDER BY tpsdsc";
	$db->monta_combo('tpsid', $sql, "S", 'Selecione', '', '', '', '', 'S', 'tpsid');

	/*Retirada Solicitada pelo Henrique - 23/12/2010
	if($ctrcontagem == "t"){
		$sql = "SELECT tpsid as codigo, tpsdsc as descricao FROM fabrica.tiposervico WHERE tpsstatus='A' and tpsid = ".TPS_PF." ORDER BY tpsdsc";
		$db->monta_combo('tpsid', $sql, "S", 'Selecione', '', '', '', '', 'S', 'tpsid');
	}else{
		$sql = "SELECT tpsid as codigo, tpsdsc as descricao FROM fabrica.tiposervico WHERE tpsstatus='A' and tpsid != ".TPS_PF." ORDER BY tpsdsc";
		echo $sql;
		$db->monta_combo('tpsid', $sql, "S", 'Selecione', '', '', '', '', 'S', 'tpsid');
	} */

}

function carregarDisciplinasProdutosPorAnalise($ansid)
{
	global $db;

	$sql = "select
				ans.ansid,
				trim(dsp.dspdsc) as disciplina,
				trim(prd.prddsc) as produto,
				trim((select tpedsc from fabrica.tipoexecucao tpe where tpe.tpeid = ser.tpeid)) as executora
			from
				fabrica.disciplina dsp
			inner join
				fabrica.servicodisciplina ser ON dsp.dspid = ser.dspid
			inner join
				fabrica.analisesolicitacao ans ON ans.ansid = ser.ansid
			left join
				fabrica.produto prd ON prd.dspid = dsp.dspid and prd.prdid in (select prdid from fabrica.servicoproduto where ansid = $ansid)
			where
				ans.ansid = $ansid
			group by
				ser.tpeid,
				dsp.dspdsc,
				prd.prddsc,
				ans.ansid
			order by
				ans.ansid,
				ser.tpeid,
				dsp.dspdsc,
				prd.prddsc";
	return $db->carregar($sql);

}

function regraEnviarDetalhamento($scsid)
{
	global $db;

	/*$sql = "SELECT count(s.scsid) as total
			FROM fabrica.analisesolicitacao a
			LEFT JOIN fabrica.solicitacaoservico s ON s.scsid=a.scsid
			WHERE a.scsid='".$_SESSION['fabrica_var']['scsid']."'
			and s.sidid is not null
			and a.tpsid is not null
			and a.ansdsc is not null
			and a.ansprevinicio is not null
			and a.ansprevtermino is not null";
    */
    $sql = "SELECT count(s.scsid) as total
            FROM fabrica.analisesolicitacao a
            INNER JOIN fabrica.solicitacaoservico s ON s.scsid=a.scsid
            INNER JOIN fabrica.fiscalsolicitacao fis ON fis.scsid=s.scsid
            WHERE a.scsid='".$_SESSION['fabrica_var']['scsid']."'
            and s.sidid is not null
            and a.tpsid is not null
            and a.ansdsc is not null
            and a.ansprevinicio is not null
            and a.ansprevtermino is not null";

	$total = $db->pegaUm($sql);

	if($total != 0) return true;

	return "Preencha todos os campos da tela de Análise preliminar.";

}


//function inserirArtefatos($ansid, $tpeid = null, $idLocal = 1) {
function inserirArtefatos($dados) {
	global $db;


	$ansid = $dados['ansid'];
	$misto = $dados['misto'];
    
	$vgcid = $dados['vgcid'];
	$mtiid = $dados['mtiid'];
	$tpsid = $dados['tpsid'];
    

        // se for manutenção
        if($tpsid > 0 && $tpsid < 5){

                // insere todos os artefatos da contratada
                if(!$misto){

                        $tpeid = '1'; //contratada

                        $sql = "DELETE FROM fabrica.servicofaseproduto WHERE ansid = ".$ansid;
                        $db->executar($sql);

                        $sql = "select fdpid FROM fabrica.fasedisciplinaproduto where fdpstatus='A' and contratada='S' order by fdpid ";
                        $artefatos = $db->carregarColuna($sql);


                        if($artefatos){
                                foreach($artefatos as $a) {
                                        //insere produtos
                                        $sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES (".$a.", {$ansid}, {$tpeid})";
                                        $db->executar($sql);
                                }
                        }

                }
                else{ // insere alguns na contratante e o restante na contratda

                        $tpeid = '2'; //contratante

                        $sql = "DELETE FROM fabrica.servicofaseproduto WHERE ansid = ".$ansid;
                        $db->executar($sql);

                        //para todas disciplinas, exceto "requisitos" e "analise e projeto" e somente a fase "concepção"
                        $sql = "select fdp.fdpid
                                        FROM fabrica.fasedisciplinaproduto fdp
                                        LEFT JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
                                        where fdp.fdpstatus='A'
                                        and misto='E'
                                        -- and fd.fasid = 1 --concepção
                                        -- and fd.dspid not in(3,4)
                                        order by fdp.fdpid ";
                        $artefatos = $db->carregarColuna($sql);

                        if($artefatos){
                                foreach($artefatos as $a) {
                                        //insere produtos
                                        $sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES (".$a.", {$ansid}, {$tpeid})";
                                        $db->executar($sql);
                                }
                        }

                        //o restante para a contratada
                        unset($artefatos);
                        $tpeid = '1'; //contratada

                        $sql = "select fdpid
                                        FROM fabrica.fasedisciplinaproduto
                                        where fdpstatus='A'
                                        and misto='A'
                                        -- and fdpid not in (select fdpid from fabrica.servicofaseproduto where ansid = ".$ansid.")
                                        order by fdpid ";
                        $artefatos = $db->carregarColuna($sql);

                        if($artefatos){
                                foreach($artefatos as $a) {
                                        //insere produtos
                                        $sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES (".$a.", {$ansid}, {$tpeid})";
                                        $db->executar($sql);
                                }
                        }

                }


        }elseif ($tpsid == 8){
            
            $tpeid = '1'; //contratada

            $sql = "DELETE FROM fabrica.servicofaseproduto WHERE ansid = ".$ansid;
            $db->executar($sql);
            
            //para as disciplinas "requisitos" "gerencia de projetos" e somente a fase "concepção" e "elaboração"
//            $sql = "select fdp.fdpid
//                            FROM fabrica.fasedisciplinaproduto fdp
//                            LEFT JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
//                            where fdp.fdpstatus='A'
//                            and (
//                            
//                                (fd.dspid = 1 and fd.fasid = 1 and prdid not in (33)) -- gerencia, concepção e produtos diferentes de (termo de abertura)
//                                or
//                                (fd.dspid = 1 and fd.fasid = 2 and prdid not in (37, 44)) -- gerencia, elaboração e produtos diferentes de (planilha de risco, serviço sem necessidade)
//                                or
//                                (fd.dspid = 3 and fd.fasid = 1 and prdid not in (22, 23, 35, 44)) -- requisitos, concepção e produtos diferentes de (documento requisitos, doc visao, glossario, serviço sem necessidade)
//                                or
//                                (fd.dspid = 3 and fd.fasid = 2 and prdid not in (40, 22, 41, 44)) -- requisitos, elaboração e produtos diferentes de (dat, documento requisitos, plan aceitação, serviço sem necessidade)
//                            )
//                            order by fdp.fdpid ";
            
            //para as disciplinas "requisitos" "gerencia de projetos" e somente a fase "concepção" e "elaboração"
            $sql = "select fdp.fdpid
                    FROM fabrica.fasedisciplinaproduto fdp
                    LEFT JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
                    where fdp.fdpstatus='A'
                        and (
                            (fd.dspid = 1 and fd.fasid = 1 and prdid not in (33))
                            or
                            (fd.dspid = 1 and fd.fasid = 2 and prdid not in (37, 44))
                            or
                            (fd.dspid = 3 and fd.fasid = 1 and prdid not in (22, 23, 35, 44))
                            or
                            (fd.dspid = 3 and fd.fasid = 2 and prdid not in (40, 22, 41, 44))
                        ) order by fdp.fdpid;";
            $artefatos = $db->carregarColuna($sql);


            if($artefatos){
                    foreach($artefatos as $a) {
                        
                            //insere produtos
                            $sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES (".$a.", {$ansid}, {$tpeid})";
                            $db->executar($sql);
                    }
            }
            
        }else{


                // insere todos os artefatos da contratada
                if(!$misto){

                        $tpeid = '1'; //contratada

                        $sql = "DELETE FROM fabrica.servicofaseproduto WHERE ansid = ".$ansid;
                        $db->executar($sql);

                        $sql = "select fdpid FROM fabrica.fasedisciplinaproduto where fdpstatus='A' order by fdpid ";
                        $artefatos = $db->carregarColuna($sql);


                        if($artefatos){
                                foreach($artefatos as $a) {
                                        //insere produtos
                                        $sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES (".$a.", {$ansid}, {$tpeid})";
                                        $db->executar($sql);
                                }
                        }

                }
                else{ // insere alguns na contratante e o restante na contratda

                        $tpeid = '2'; //contratante

                        $sql = "DELETE FROM fabrica.servicofaseproduto WHERE ansid = ".$ansid;
                        $db->executar($sql);

                        //para todas disciplinas, exceto "requisitos" e "analise e projeto" e somente a fase "concepção"
                        $sql = "select fdp.fdpid
                                        FROM fabrica.fasedisciplinaproduto fdp
                                        LEFT JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
                                        where fdp.fdpstatus='A'
                                        and fd.fasid = 1 --concepção
                                        and fd.dspid not in(3,4)
                                        order by fdp.fdpid ";
                        $artefatos = $db->carregarColuna($sql);

                        if($artefatos){
                                foreach($artefatos as $a) {
                                        //insere produtos
                                        $sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES (".$a.", {$ansid}, {$tpeid})";
                                        $db->executar($sql);
                                }
                        }


                        //somente para fase "Requisitos"
                        unset($artefatos);
                        $sql = "select fdp.fdpid
                                        FROM fabrica.fasedisciplinaproduto fdp
                                        LEFT JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
                                        where fdp.fdpstatus='A'
                                        and fd.dspid in(3)
                                        order by fdp.fdpid ";
                        $artefatos = $db->carregarColuna($sql);

                        if($artefatos){
                                foreach($artefatos as $a) {
                                        //insere produtos
                                        $sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES (".$a.", {$ansid}, {$tpeid})";
                                        $db->executar($sql);
                                }
                        }


                        //o restante para a contratada
                        unset($artefatos);
                        $tpeid = '1'; //contratada

                        $sql = "select fdpid
                                        FROM fabrica.fasedisciplinaproduto
                                        where fdpstatus='A'
                                        and fdpid not in (select fdpid from fabrica.servicofaseproduto where ansid = ".$ansid.")
                                        order by fdpid ";
                        $artefatos = $db->carregarColuna($sql);

                        if($artefatos){
                                foreach($artefatos as $a) {
                                        //insere produtos
                                        $sql = "INSERT INTO fabrica.servicofaseproduto(fdpid, ansid, tpeid) VALUES (".$a.", {$ansid}, {$tpeid})";
                                        $db->executar($sql);
                                }
                        }

                }


                //deleta os artefatos prdid=44, quando uma fase possui varios artefatos.
                $fdpidArray = array();
                $disciplina = $db->carregarColuna("select dspid from fabrica.disciplina where dspstatus='A'");

                if($disciplina){
                        foreach($disciplina as $d){

                                $fase = $db->carregarColuna("select fasid from fabrica.fase where fasstatus='A'");

                                foreach($fase as $f){

                                        $dados = $db->carregar("select fdpid, prdid FROM fabrica.fasedisciplinaproduto f
                                                                                        inner join fabrica.fasedisciplina fd ON fd.fsdid=f.fsdid
                                                                                        where f.fdpstatus='A' and dspid=$d and fasid=$f");

                                        if(count($dados) > 1){
                                                foreach($dados as $dd) {
                                                        if($dd['prdid'] == '44') $fdpid = $dd['fdpid'];
                                                }
                                                array_push($fdpidArray, $fdpid);
                                        }

                                }

                        }

                        $sql = "DELETE FROM fabrica.servicofaseproduto WHERE ansid = ".$ansid." and fdpid in(".implode(',',$fdpidArray).")" ;
                        $db->executar($sql);
                }

        }// fim if tipo manutenção

    if($vgcid){
    	$sql = "UPDATE fabrica.analisesolicitacao SET vgcid=$vgcid WHERE ansid=$ansid";
    	$db->executar($sql);
    }
	if($mtiid){
    	$sql = "UPDATE fabrica.analisesolicitacao SET mtiid=$mtiid WHERE ansid=$ansid";
    	$db->executar($sql);
    }
	if($tpsid){
    	$sql = "UPDATE fabrica.analisesolicitacao SET tpsid=$tpsid WHERE ansid=$ansid";
    	$db->executar($sql);
    }

	$db->commit();

	echo "OK";
	exit;

	/*

		$btnSalvar = false;
		$fase = 1;
		//verifica se esta checado com o contratante ou contratada
		if($tpeid == '1') $tpeidAux = '2';
		else $tpeidAux = '1';

		$verificaTipo = "";
		$sql = "SELECT count(sp.fdpid) as total
				FROM fabrica.servicofaseproduto sp
				INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
				INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
				INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
				WHERE sp.ansid = {$ansid}
				and fd.fasid = {$fase}
				and fd.dspid = {$dspid}
				and sp.tpeid = {$tpeidAux}
				and fdp.fdpstatus = 'A'";
		$total = $db->pegaUm($sql);
		if($total > 0) $verificaTipo = "disabled";

		$sql = "SELECT
							f.fdpid AS codigo,
							p.prddsc AS descricao
						FROM
							fabrica.fasedisciplinaproduto f
						INNER JOIN
							fabrica.fasedisciplina fd ON fd.fsdid = f.fsdid
						INNER JOIN
							 fabrica.produto p ON p.prdid = f.prdid
						WHERE
							p.prdstatus='A'
							and f.fdpstatus = 'A'
							AND fd.fasid = {$fase}
							AND fd.dspid = ".$dspid."
							order by 2";
				//dbg($sql,1);
		$dados = $db->carregar($sql);
		$i=0;
		if($dados){

			echo '<input type="checkbox" name="todos_'.$fase.'" id="todos_'.$fase.'" '.$verificaTipo.' value="" onclick="marcaTodos('.$fase.',this)">&nbsp;Todos<br>';
			echo '<div id="divMarcaTodos_'.$fase.'">';

			foreach($dados as $dado){

						//verifica se esta checado
				$checadoSim = "";
				$sql = "SELECT sp.fdpid
								FROM fabrica.servicofaseproduto sp
								INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
								INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
								INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
								WHERE sp.ansid = {$ansid}
								and fdp.fdpstatus = 'A'
								and fd.fasid = {$fase}
								and fd.dspid = {$dspid}
								and sp.tpeid = {$tpeid}
								and sp.fdpid = {$dado['codigo']}";
				$checado = $db->pegaUm($sql);

				if($checado) $checadoSim = "checked";

						//ordem id = id_produto - id_disciplina - id_contratante/contratada - id_fase
				echo '<input type="checkbox" name="fdpid[]" id="fdpid" value="'.$dados[$i]['codigo'].'" '.$checadoSim.' '.$verificaTipo.'>&nbsp;'.$dados[$i]['descricao'].'<br>';

						//habilita botao salvar
				if(!$verificaTipo) $btnSalvar = true;

				$i++;
			}

			echo '</div>';

		} else {
			echo '<div style="text-align:center;color:red;">Nenhum registro encontrado</div>';
		}

	*/

}

function regraReabrirUltimaSituacaoSS($scsid)
{
    global $db;


    // seleciona ultimo estado valido do fluxo (antes do cancelamento/finalização)
    $sql = "SELECT
                wkd.docid,
                aed.aedid,
                aed.esdidorigem
            FROM
                fabrica.solicitacaoservico as fss
            INNER JOIN
                workflow.historicodocumento wkd
                ON wkd.docid = fss.docid
            INNER JOIN
                workflow.acaoestadodoc as aed
                ON aed.aedid = wkd.aedid
            WHERE
                fss.scsid = {$scsid}
            ORDER BY
                wkd.htddata DESC
            LIMIT 2";
    $total = $db->carregar($sql);

    $docid = $total[1]['docid'];
    $esdidOrigem = $total[1]['esdidorigem'];

    //recuperando estado atual
    $sql = "SELECT
                doc.esdid
            FROM
                fabrica.solicitacaoservico scs
            INNER JOIN
                workflow.documento doc
                ON doc.docid=scs.docid
            WHERE
                scsid=$scsid";
    $esdidAtual = $db->pegaUm($sql);

    // selecionando a acao pelo estado de origem e estado de destino
    $sql = "SELECT
                aed.aedid
            FROM
                workflow.acaoestadodoc aed
            WHERE
                aed.esdiddestino={$esdidOrigem}
            AND
                aed.esdidorigem={$esdidAtual}";

    $aedid = $db->pegaUm($sql);

    //return wf_alterarEstado($docid, $aedid, '', array());
	return true;

}


function regraReabrirUltimaSituacaoOS($odsid)
{
    global $db;


    // seleciona ultimo estado valido do fluxo (antes do cancelamento/finalização)
    $sql = "SELECT
                wkd.docid,
                aed.aedid,
                aed.esdidorigem
            FROM
                fabrica.ordemservico as ods
            INNER JOIN
                workflow.historicodocumento wkd
                ON wkd.docid = ods.docid
            INNER JOIN
                workflow.acaoestadodoc as aed
                ON aed.aedid = wkd.aedid
            WHERE
                ods.odsid = {$odsid}
            ORDER BY
                wkd.htddata DESC
            LIMIT 2";

    $total = $db->carregar($sql);

    $docid = $total[1]['docid'];
    $esdidOrigem = $total[1]['esdidorigem'];

    //recuperando estado atual
    $sql = "SELECT
                doc.esdid
            FROM
                fabrica.ordemservico ods
            INNER JOIN
                workflow.documento doc
                ON doc.docid=ods.docid
            WHERE
                odsid=$odsid";
    $esdidAtual = $db->pegaUm($sql);

    // selecionando a acao pelo estado de origem e estado de destino
    $sql = "SELECT
                aed.aedid
            FROM
                workflow.acaoestadodoc aed
            WHERE
                aed.esdiddestino={$esdidOrigem}
            AND
                aed.esdidorigem={$esdidAtual}";

    $aedid = $db->pegaUm($sql);

    return wf_alterarEstado($docid, $aedid, '', array());

}


function regraReabrirUltimaSituacaoOSPF($odsid)
{
    global $db;


    // seleciona ultimo estado valido do fluxo (antes do cancelamento/finalização)
    $sql = "SELECT
                wkd.docid,
                aed.aedid,
                aed.esdidorigem
            FROM
                fabrica.ordemservico as ods
            INNER JOIN
                workflow.historicodocumento wkd
                ON wkd.docid = ods.docidpf
            INNER JOIN
                workflow.acaoestadodoc as aed
                ON aed.aedid = wkd.aedid
            WHERE
                ods.odsid = {$odsid}
            ORDER BY
                wkd.htddata DESC
            LIMIT 2";

    $total = $db->carregar($sql);

    $docid = $total[1]['docid'];
    $esdidOrigem = $total[1]['esdidorigem'];

    //recuperando estado atual
    $sql = "SELECT
                doc.esdid
            FROM
                fabrica.ordemservico ods
            INNER JOIN
                workflow.documento doc
                ON doc.docid=ods.docidpf
            WHERE
                odsid=$odsid";
    $esdidAtual = $db->pegaUm($sql);

    // selecionando a acao pelo estado de origem e estado de destino
    $sql = "SELECT
                aed.aedid
            FROM
                workflow.acaoestadodoc aed
            WHERE
                aed.esdiddestino={$esdidOrigem}
            AND
                aed.esdidorigem={$esdidAtual}";

    $aedid = $db->pegaUm($sql);

    return wf_alterarEstado($docid, $aedid, '', array());

}


?>