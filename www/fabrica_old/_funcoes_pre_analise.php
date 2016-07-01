<?
function regraEnviarParaAnalise( $scsid )
{
	global $db;

	$sql = "SELECT count(s.scsid) as total
			FROM fabrica.analisesolicitacao a
			LEFT JOIN fabrica.solicitacaoservico s ON s.scsid=a.scsid
			WHERE a.scsid='".$_SESSION['fabrica_var']['scsid']."'
			and s.sidid is not null
			and a.tpsid is not null
			and a.ansdsc is not null
			and a.ansprevinicio is not null
			and a.ansprevtermino is not null";

	$total = $db->pegaUm($sql);

	if($total != 0) return true;

	return "Preencha todos os campos da tela de Pré-análise.";

}

function atualizarPreAnaliseSolicitacaoServico($dados) {
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
		
		if($_SERVER['HTTP_HOST'] == 'simec.mec.gov.br'){
			enviar_email($remetente, $destinatarios, $assunto, $conteudo);
		}
		
	}

	//Se o tipo de serviço for CONTAGEM DE PONTO DE FUNÇÃO, a empresa é a que estiver com a flag 'ctrcontagem' da tabela 'fabrica.contrato' ativa.
	/*
	if ($_REQUEST['ctrid']){
            $ctrid = $_REQUEST['ctrid'];
        }else{
            
            if( $dados['tpsid'] ){
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

                $dados['ctrid'] = $db->pegaUm($sql);
            }
	}
    */    
	
	//recupera contrato
	$sql = " SELECT ctrid FROM fabrica.vigenciacontrato where vgcid = ".$dados['vgcid'];
    $dados['ctrid'] = $db->pegaUm($sql);
	

	$sql = "UPDATE fabrica.analisesolicitacao
                SET vgcid       = {$dados['vgcid']}
                ,	mtiid       = {$dados['mtiid']}
                ,	tpsid       = {$dados['tpsid']}
                , ansgarantia   = {$dados['ansgarantia']}
                , mensuravel    = {$dados['ansmensuravel']}
                , ansdsc        = '{$dados['ansdsc']}'
                , ansprevinicio = '{$ansprevinicio}'
                , ansprevtermino= '{$ansprevtermino}'
                , ansqtdpf      = ".(($dados['ansqtdpf'])?"'".$dados['ansqtdpf']."'":"NULL")."
                , odsidpf       = ".(($dados['odsidpf'])?"'".$dados['odsidpf']."'":"NULL")."
                , ctrid         = ".($dados['ctrid'] ? $dados['ctrid'] : $ctrid)."
 			WHERE ansid = '".$dados['ansid']."';";

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
	if((int)$dados['tpsid'] > 5){
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

    /*
     * Quando selecionado o campo Serviço em Garantia, como SIM, 
     * o campo O.S. garantia é habilitado para vinculação de uma OS finalizada dentro de um período de um ano
     * a partir da data de abertura
     */
	if( $dados['odsidorigem'] ) 
        $db->executar("UPDATE fabrica.solicitacaoservico SET odsidorigem = '".$dados['odsidorigem']."' WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'");

	$db->commit();

	echo "<script>
			alert('Pré-análise de solicitação de serviço atualizada com sucesso');
			window.location='fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid=".$_SESSION['fabrica_var']['scsid']."';
		  </script>";
}
?>
