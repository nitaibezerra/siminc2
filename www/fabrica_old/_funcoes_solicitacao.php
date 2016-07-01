<?php
//problema svn
function downloadAnexoSolicitacaoServico($dados) {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("anexosolicitacao", NULL ,"fabrica");
	$file->getDownloadArquivo($dados['arqid']);
}


function telaBuscarUsuarios($dados) {
	global $db;

	echo "<table class=listagem width=100%>";
	echo "<tr>";
	echo "<td class=SubTituloCentro colspan=2>Lista de requisitantes</td>";
	echo "</tr>";

	echo "<tr>";
	echo "<td class=SubTituloDireita><b>Requisitante:</b></td>";
	echo "<td>";

	$sql = "SELECT
			 u.usucpf AS codigo,
			 u.usunome AS descricao
			FROM
			 seguranca.usuario u
			 inner join seguranca.usuario_sistema us on
			 u.usucpf = us.usucpf
			 where
			 us.sisid = ".$_SESSION['sisid']." AND
			 us.susstatus = 'A' AND
			 us.suscod = 'A'
			ORDER BY
			 TRANSLATE(u.usunome, ' ','0')";

	$db->monta_combo('usucpf', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'usucpf');

	echo "</td></tr>";
	echo "<tr><td class=SubTituloDireita colspan=2><input type=button name=selecionarequisitante value=Selecionar onclick=selecionarRequisitante();> <input type=button name=fechar value=Fechar onclick=closeMessage();></td></tr>";
	echo "</table>";

}

function pegarUnidadeUsuario($dados) {
	global $db;
	/*
	echo $db->pegaUm("SELECT UPPER(uni.unidsc) as unidsc
					  FROM seguranca.usuario usu
					  LEFT JOIN public.unidade uni ON uni.unicod = usu.unicod
					  WHERE usucpf='".$dados['usucpf']."'");
	*/
	$ug = $db->pegaUm("SELECT UPPER(ung.ungdsc) as unidsc
					  FROM seguranca.usuario usu
					  LEFT JOIN public.unidadegestora ung ON ung.ungcod = usu.ungcod
					  WHERE usucpf='".$dados['usucpf']."'");

	echo (($ug)?$ug:"NÃO CADASTRADO");


}

function inserirSolicitacaoServico($dados) {
	global $db;

	if( $dados['prg'] )
	{
		// extraindo a variavel prg em três variaveis divididas por "_"
		$prg = explode("_", $dados['prg']);
		$prgid  = $prg[0];
		$prgcod = $prg[1];
		$prgano = $prg[2];
	}

	// formatando para a forma YYYY-mm-dd
	$scsprevatendimento = formata_data_sql($dados['scsprevatendimento']);

	$sql = "INSERT INTO fabrica.solicitacaoservico(
            prgid, prgcod, prgano, usucpfrequisitante,
            scsnecessidade, scsjustificativa, scsprevatendimento,
            scsstatus, usucpforigem, sidid, dataabertura)
		    VALUES (".(($prgid)?"'".$prgid."'":"NULL").", ".(($prgcod)?"'".$prgcod."'":"NULL").", ".(($prgano)?"'".$prgano."'":"NULL").", '{$dados['usucpfrequisitante']}',
		    		'{$dados['scsnecessidade']}', '{$dados['scsjustificativa']}', '{$scsprevatendimento}', 'A', '{$dados['usucpforigem']}', ".(($dados['sidid'])?$dados['sidid']:"NULL").", NOW() ) RETURNING scsid;";

	$dados['scsid'] = $db->pegaUm($sql);
	// cadastrando o documento caso não possua
	if(!pegarDocidSolicitacaoServico($dados)){

		$docdsc = "Fluxo da solicitação de serviço - ID " . $dados['scsid'];
		// cria documento
		$docid = wf_cadastrarDocumento(WORKFLOW_SOLICITACAO_SERVICO, $docdsc );
		$sql = "UPDATE fabrica.solicitacaoservico SET docid='".$docid."' WHERE scsid='".$dados['scsid']."'";
		$db->executar($sql);

	}
	// inserindo o anexo referente a solicitação
	inserirAnexoSolicitacao($dados);

	$db->commit();

	//envia email
	enviaEmailCadSolicitacao($dados['scsid'], $acao = 'INCLUIR');

	echo "<script>
			alert('Sua solicitação de nº ".$dados['scsid']." foi cadastrada com sucesso e será analisada.');
			window.location='fabrica.php?modulo=principal/listarSolicitacoes&acao=A';
		  </script>";

}

function inserirAnexoSolicitacao($dados) {

	if($_FILES['arquivo']['error'] == 0) {

		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

		$campos = array("scsid"         => "'".$dados['scsid']."'",
						"usucpf"        => "'".$_SESSION['usucpf']."'",
						"tasid"         => "'".$dados['tasid']."'",
						"ansdsc"        => "'".$dados['ansdsc_']."'",
						"ansdtinclusao" => "NOW()",
						"ansstatus"     => "'A'");

		$file = new FilesSimec("anexosolicitacao", $campos ,"fabrica");
		$file->setUpload( (($dados['ansdsc_'])?$dados['ansdsc_']:NULL), $key = "arquivo" );

		if($dados['redirecionamento']) {

			echo "<script>
					alert('Arquivo anexado com sucesso');
					window.location='".$dados['redirecionamento']."';
				  </script>";

		}

	}
}

function telaCabecalhoSolicitacaoServico($dados) {
	global $db;

	if($dados['scsid']) {
		$sql = "SELECT
					s.scsid,
					u.usunome,
					s.scsnecessidade,
					s.scsjustificativa,
					p.prgdsc,
					un.unidsc,
					s.scsprevatendimento,
					s.dataabertura,
									os.ctrid,
									(CASE WHEN c.ctrtipoempresaitem = 1 THEN
									   'Empresa do Item 1'
										WHEN c.ctrtipoempresaitem = 2 THEN
										'Empresa do Item 2'
									END) as entnome,
									a.mensuravel,
					sis.siddescricao as sistema
					--sis.sidabrev || ' - ' || sis.siddescricao as sistema
				FROM fabrica.solicitacaoservico s
				LEFT JOIN seguranca.usuario u ON u.usucpf=s.usucpfrequisitante
				LEFT JOIN public.unidade un ON un.unicod=u.unicod
				LEFT JOIN monitora.programa p ON p.prgid=s.prgid AND p.prgcod=s.prgcod AND p.prgano=s.prgano
				LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = s.sidid
							LEFT JOIN fabrica.ordemservico os ON os.scsid = s.scsid
							LEFT JOIN fabrica.analisesolicitacao a ON a.scsid = s.scsid
							LEFT JOIN fabrica.contrato c  ON c.ctrid = a.ctrid
				LEFT JOIN entidade.entidade ent on ent.entid = c.entidcontratado and ent.entstatus='A'and c.ctrstatus='A'
				WHERE s.scsid='" . $dados['scsid'] . "'";

		$solicitacaoservico = $db->pegaLinha($sql);
	}

	
	if($solicitacaoservico) {

	//  Pegar Situação da SS
$sql = "SELECT  wkd.esdid,
                ans.ansid
                            FROM fabrica.solicitacaoservico as fss

                                INNER JOIN  workflow.documento as wkd
                                    on wkd.docid = fss.docid
                                INNER JOIN  workflow.estadodocumento as wed
                                    on wed.esdid = wkd.esdid
                                LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = fss.scsid

                        WHERE fss.scsid = {$solicitacaoservico['scsid']}";
$situacaoSolicitacao = $db->pegaLinha($sql);

        $link = "<span style='cursor:pointer; color: #0066CC;' onclick=window.location.href='fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid=".$solicitacaoservico['scsid']."&ansid=".$situacaoSolicitacao['ansid']."'>".$solicitacaoservico['scsid']."</span>";

		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
		echo "<tr><td colspan=3 class=SubTituloEsquerda>Dados da solicitação de serviço</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Nº SS:</td><td width=60%>".$link."</td><td width=10px;>&nbsp;</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Requisitante:</td><td width=60%>".$solicitacaoservico['usunome']."</td><td width=10px;>&nbsp;</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Unidade do requisitante:</td><td width=60%>".$solicitacaoservico['unidsc']."</td><td width=10px;>&nbsp;</td></tr>";
		if($solicitacaoservico['sistema']){
			echo "<tr><td class=SubTituloDireita width=40%>Sistema:</td><td width=60%>".$solicitacaoservico['sistema']."</td><td width=10px;>&nbsp;</td></tr>";
		}
		echo "<tr><td class=SubTituloDireita width=40%>Necessidade:</td><td width=60%>".$solicitacaoservico['scsnecessidade']."</td><td width=10px;>&nbsp;</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Justificativa:</td><td width=60%>".$solicitacaoservico['scsjustificativa']."</td><td width=10px;>&nbsp;</td></tr>";

		$sql = "SELECT ar.arqid, an.ansdsc||' ('||ar.arqnome||'.'||ar.arqextensao||')' as nomearquivo FROM fabrica.anexosolicitacao an
				LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
				WHERE scsid='".$dados['scsid']."' AND ansstatus='A'";

		$anexossol = $db->carregar($sql);
		if($anexossol[0]) {
			foreach($anexossol as $ane) {
				$arqs[] = "<a href=fabrica.php?modulo=principal/abrirSolicitacao&acao=A&requisicao=downloadAnexoSolicitacaoServico&arqid=".$ane['arqid'].">".$ane['nomearquivo']."</a>";
			}
		}
		echo "<tr><td class=SubTituloDireita width=40%>Arquivos anexados:</td><td width=60%>".(($arqs)?implode("<br/>", $arqs):"Nenhum aquivo anexado")."</td><td width=10px;>&nbsp;</td></tr>";
		echo "<tr><td class=SubTituloDireita width=40%>Data de abertura:</td><td width=60%>".formata_data($solicitacaoservico['dataabertura'])."</td><td width=10px;>&nbsp;</td></tr>";


		// analisando data de previsão
		include APPRAIZ ."includes/classes/dateTime.inc";
		if($solicitacaoservico['scsprevatendimento'] > date("Y-m-d")) {
			if(Data::subtraiDiasNaData(str_replace("-","",$solicitacaoservico['scsprevatendimento']),2) <= date("Ymd")) {
				$scsprevatendimento = "<font color=#FDD017>".formata_data($solicitacaoservico['scsprevatendimento'])."</span>";
			} else {
				$scsprevatendimento = "<font color=green>".formata_data($solicitacaoservico['scsprevatendimento'])."</span>";
			}
		} else {
			$scsprevatendimento = "<font color=#E41B17>".formata_data($solicitacaoservico['scsprevatendimento'])."</font>";
		}
		echo "<tr><td class=SubTituloDireita width=40%>Expectativa de atendimento:</td><td width=60%>".$scsprevatendimento."</td><td width=10px;>&nbsp;</td></tr>";
		if ($solicitacaoservico['entnome'] != NULL) {
			echo "<tr><td class=SubTituloDireita width=40%>Contrato:</td><td align=60%>".$solicitacaoservico['entnome']."</td><td style=width:5px;>&nbsp;</td></tr>";
			echo "<tr><td class=SubTituloDireita width=40%>Mensurável:</td><td align=60%>".(($solicitacaoservico['mensuravel']=="t")?"Sim":"Não")."</td><td style=width:5px;>&nbsp;</td></tr>";
		}
		echo "</table>";

	}

}

function removerSolicitacaoServico($dados) {
	global $db;
	$sql = "UPDATE fabrica.solicitacaoservico SET scsstatus='I' WHERE scsid='".$dados['scsid']."'";
	$db->executar($sql);
	$db->commit();

	echo "<script>
			alert('Solicitação de serviço removida com sucesso');
			window.location='fabrica.php?modulo=principal/listarSolicitacoes&acao=A';
		  </script>";
}

function filtraContratoPorSistema($sidid){
	global $db;
	$sql = "select
					ctrid as codigo,
					ctrnumero as descricao
				from
					fabrica.contrato
				where
					ctrid in (select distinct ctrid from fabrica.contratosistema where ctsstatus = 'A' and sidid = {$sidid['sidid']})
				order by
					ctrnumero";
	$db->monta_combo("ctrid",$sql,"S","Selecione...","","","","","S","ctrid","");
}

function filtraItem($dados){
	global $db;
	
	if($dados['vgcid']){
    	$andItem = " AND vc.vgcid = ".$dados['vgcid'];
    }
    else{
    	$andItem = " AND vc.vgcid = 0";
    }
              		
    $sql = "select distinct
					mi.mtiid as codigo,
					mt.mtcsigla ||' - '|| mi.mtinome as descricao
			from 
					fabrica.metricaitem mi
			inner join fabrica.metrica mt on mt.mtcid = mi.mtcid
			inner join fabrica.metricaitemcontrato mc on mc.mtiid = mi.mtiid 
			inner join fabrica.vigenciacontratometricaitem vc on vc.mtiid = mc.mtiid
			where 
				mi.mtistatus = 'A'
				$andItem
			";
     		
     $db->monta_combo("mtiid",$sql, "S","-- Selecione --",'filtraTipoServico','','', '', "S", 'mtiid','','');      
                           
}


function filtraTipoServico($dados){
	global $db;
	
	if($dados['mtiid']){
    	$andTipo = " AND vc.mtiid = ".$dados['mtiid'];
    }
    else{
    	$andTipo = " AND vc.mtiid = 0";
    }
              		
	$sql = "SELECT 
				ts.tpsid as codigo, ts.tpsdsc as descricao 
		    FROM fabrica.tiposervico ts
		    inner join fabrica.vigenciacontratometricaitem vc on vc.mtiid = ts.mtiid
		    WHERE ts.tpsstatus='A' 
		    $andTipo
		    group by ts.tpsid, ts.tpsdsc
		    ORDER BY tpsdsc";
	$db->monta_combo('tpsid', $sql, "S", '-- Selecione --', 'mostraDisciplina', '', '', '', 'S', 'tpsid');
				
                           
}

function pegarDocidSolicitacaoServico($dados) {
	global $db;
	$sql = "SELECT docid FROM fabrica.solicitacaoservico WHERE scsid = '".$dados['scsid']."'";
	return (integer) $db->pegaUm( $sql );
}

function atualizarSolicitacaoServico($dados) {
	global $db;

	$prg = explode("_", $dados['prg']);
	$prgid  = $prg[0];
	$prgcod = $prg[1];
	$prgano = $prg[2];

	$scsprevatendimento = formata_data_sql($dados['scsprevatendimento']);

	$sql = "UPDATE fabrica.solicitacaoservico
   			SET usucpfrequisitante='{$dados['usucpfrequisitante']}',
       			scsnecessidade='{$dados['scsnecessidade']}', scsjustificativa='{$dados['scsjustificativa']}', scsprevatendimento='{$scsprevatendimento}', sidid=".(($dados['sidid'])?$dados['sidid']:"NULL")."
       		WHERE scsid='".$dados['scsid']."'";

	$db->executar($sql);
	$db->commit();

	//envia email
	enviaEmailCadSolicitacao($dados['scsid'], $acao = 'ALTERAR');

	echo "<script>
			alert('Solicitação de serviço atualizada com sucesso');
			window.location='fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid=".$dados['scsid']."';
		  </script>";
}

function removerAnexoSolicitacao($dados) {
	global $db;

	$sql = "UPDATE fabrica.anexosolicitacao SET ansstatus='I' WHERE ansid='".$dados['ansid']."'";
	$db->executar($sql);
	$db->commit();

	echo "<script>
			alert('Anexo removido com sucesso');
			window.location='".$_SERVER['HTTP_REFERER']."';
		  </script>";
}

function salvarContagemPFOS($dados)
{
	global $db;
	$dados = $_REQUEST;

	/*
	$dadosctr = $db->pegaLinha("SELECT c.ctrid, c.ctrqtdpfalocado, c.ctrqtdpfcontrato
								FROM fabrica.contrato c WHERE ctrcontagem=TRUE");

	if($dadosctr) {
		if($dadosctr['ctrqtdpfcontrato'] < ($dadosctr['ctrqtdpfalocado']+(($dados['odsqtdpfestimada'])?$dados['odsqtdpfestimada']:"0"))) {

			die("<script>
					alert('O contrato não possui pontos de função suficiente');
					window.location='fabrica.php?modulo=principal/abrirSolicitacao&acao=A';
		  		 </script>");

		}
	}
	*/
	
	//verifica saldo na vigencia do contrato
	if($_SESSION['fabrica_var']['ansid']){
	    $sql = "select distinct
		                vi.vcmid, vi.vcmvolumecontratado, vi.vcmvolumeutilizado
		          from 
		                fabrica.vigenciacontratometricaitem vi
		          inner join fabrica.vigenciacontrato vc on vc.vgcid = vi.vgcid and vc.vgcstatus='A'
		          inner join fabrica.analisesolicitacao an on an.vgcid = vi.vgcid and an.mtiid = vi.mtiid
		          where an.ansid=".$_SESSION['fabrica_var']['ansid'];
	    $saldo = $db->pegaLinha($sql);
	    $vcmid = $saldo['vcmid'];
		$vcmvolumecontratado = $saldo['vcmvolumecontratado'] ? $saldo['vcmvolumecontratado'] : 0; 
		$vcmvolumeutilizado = $saldo['vcmvolumeutilizado'] ? $saldo['vcmvolumeutilizado'] : 0;
		
		if($vcmid){
		    if ( $vcmvolumecontratado < ($vcmvolumeutilizado + (($vlEstimada) ? $vlEstimada : 0)) ) {
		
			    die( "<script>
					alert('O contrato não possui pontos de função suficiente');
					window.location='fabrica.php?modulo=principal/cadDetalhamento&acao=A';
		 		 </script>" );
		    }
		    
		    //atualiza saldo
		    //$db->executar("UPDATE fabrica.vigenciacontratometricaitem SET vcmvolumeutilizado=COALESCE(vcmvolumeutilizado,0)+" . (($vlEstimada) ? $vlEstimada : "0") . " WHERE vcmid='" . $vcmid . "'" );
		}	
	}
	
	if(!$dados['odsid']){
		$arrRes['msg'] = "Não foi possível realizar a operação!";
		return $arrRes;
	}else{

		$sql = "UPDATE fabrica.ordemservico
   			SET
   				odssubtotalpf=".($dados['odssubtotalpf'] ? "'".str_replace(',','.',str_replace('.','',$dados['odssubtotalpf']))."'" : "NULL").",
   				odsqtdpfestimada=".($dados['odsqtdpfestimada'] ? "'".str_replace(',','.',str_replace('.','',$dados['odsqtdpfestimada']))."'" : "NULL").",
   				odsqtdpfdetalhada=".($dados['odsqtdpfdetalhada'] ? "'".str_replace(',','.',str_replace('.','',$dados['odsqtdpfdetalhada']))."'" : "NULL")."
 			WHERE odsid='".$dados['odsid']."';";

		$db->executar($sql);

		/*
		if($dados['odsqtdpfestimada']) {
			$dif = $dados['odsqtdpfestimada']-$dados['odsqtdpfestimada_'];
			$db->executar("UPDATE fabrica.contrato SET ctrqtdpfalocado=COALESCE(ctrqtdpfalocado,0)".(($dif >= 0)?"+".$dif:"-".($dif*-1))." WHERE ctrid='".$dadosctr['ctrid']."'");
		} elseif($dados['odsqtdpfdetalhada']) {
			$dif = $dados['odsqtdpfdetalhada']-$dados['odsqtdpfdetalhada_'];
			$db->executar("UPDATE fabrica.contrato SET ctrqtdpfalocado=COALESCE(ctrqtdpfalocado,0)".(($dif >= 0)?"+".$dif:"-".($dif*-1))." WHERE ctrid='".$dadosctr['ctrid']."'");
		}
		*/

		if($_FILES['arquivo'] && $_FILES['arquivo']['error'] == 0) {

			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

			$campos = array("taoid"         => "'".$dados['taoid']."'",
							"odsid"         => "'".$dados['odsid']."'",
							"aosdsc"        => "'".$dados['aosdsc_']."'",
							"aosdtinclusao" => "NOW()",
							"aosstatus"     => "'A'");

			$file = new FilesSimec("anexoordemservico", $campos ,"fabrica");
			$file->setUpload( (($dados['aosdsc_'])?$dados['aosdsc_']:NULL), $key = "arquivo" );

		}
		$db->commit();
		$arrRes['msg'] = "Operação realizada com sucesso!";
		return $arrRes;
	}
}

function excluirAnexo()
{
	global $db;

	$dados = $_REQUEST;

	if(!$dados['arqid']){
		$arrRes['msg'] = "Não foi possível excluir o anexo!";
		return $arrRes;
	}

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	$campos = array("taoid"         => "'".$dados['taoid']."'",
					"odsid"         => "'".$dados['odsid']."'",
					"aosdsc"        => "'".$dados['aosdsc_']."'",
					"aosdtinclusao" => "NOW()",
					"aosstatus"     => "'A'");

	$file = new FilesSimec("anexoordemservico", $campos ,"fabrica");
	$file->setRemoveUpload($dados['arqid']);
	$arrRes['msg'] = "Anexo excluído com sucesso!";
	return $arrRes;
}

/**
 * Função utilizada no workflow que verifica se todos os produtos esperados foram especificados
 * se todos os protutos esperados estiverem sido analisados então libera para análise
 *
 */
function verificaProdutosEsperados(){
	global $db;
	$sql = "select count(*) from fabrica.ordemservico where scsid = {$_SESSION['fabrica_var']['scsid']}";
	$dados = $db->pegaUm($sql);
	
	
	if($dados > 0){
		return verificaAnexoEstimada();//CHAMA A FUNÇÃO PARA VALIDAR SE POSSUI ANEXO DO TIPO DE RELATORIO DE CONTAGEM ESTIMADA.
	}else{
		return "É necessário cadastrar pelo menos uma Ordem de Serviço.";
	}

	/*
	// produtos esperados
	$sql = "SELECT
				COUNT(p.prddsc)
			FROM fabrica.servicoproduto s
			LEFT JOIN fabrica.produto p ON s.prdid=p.prdid
			WHERE s.ansid='{$_SESSION['fabrica_var']['ansid']}'";

	// quantidade de produtos esperados
	$qtdProd = $db->pegaUm($sql);

	// verificando os ids das análises cadastradas
	$sql = "SELECT
				odsid
			FROM fabrica.ordemservico
			WHERE
				scsid='{$_SESSION['fabrica_var']['scsid']}'
			ORDER BY odsid";

	$odsids = $db->carregarColuna($sql);

	$qtdProdCadastrado = 0;
	foreach ($odsids as $odsid) {
		$sql = "SELECT
					COUNT(p.prdid)
				FROM fabrica.produto p
				LEFT JOIN fabrica.ordemservicoproduto o ON o.prdid=p.prdid
				WHERE odsid='{$odsid}'";

		$qtdProdCadastrado += $db->pegaUm($sql);

	}

	if($qtdProdCadastrado >= $qtdProd){
		return true;
	}else{
		return "Os produtos esperados não foram realizados";
	}
	*/
}

function verificaAnexoEstimada(){
	global $db;
    
	//se for UST, retorna true
	//recupera sigla do item da metrica
	$sigla = recuperaMetrica( $_SESSION['fabrica_var']['ansid'] );
	if($sigla == 'UST'){
		return true;
	}
	
    $sqlAnaliseSolicitacaoEmGarantia  = "SELECT ans.ansid, ss.scsid 
                                FROM fabrica.analisesolicitacao ans
                                INNER JOIN fabrica.solicitacaoservico ss
                                    ON ans.scsid = ss.scsid
                                INNER JOIN workflow.documento doc
                                ON ss.docid = doc.docid
                                INNER JOIN workflow.estadodocumento esddoc
                                    ON doc.esdid = esddoc.esdid
                                WHERE ans.ansgarantia = 't'
                                AND ss.scsid = {$_SESSION['fabrica_var']['scsid']}";
                                
    $dadosAnaliseEmGarantia = $db->pegaUm($sqlAnaliseSolicitacaoEmGarantia);
    
    if( $dadosAnaliseEmGarantia != false )
    {
        return true;
    }
    
	
	$sqlAnexo = "SELECT count(*)
						FROM fabrica.anexoordemservico an
						LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid
						LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
						LEFT JOIN fabrica.ordemservico fa ON fa.odsid=an.odsid
					WHERE scsid= {$_SESSION['fabrica_var']['scsid']} AND aosstatus='A' AND tp.taoid = 28";
		$dadosAnexo = $db->pegaUm($sqlAnexo);
		if($dadosAnexo > 0){
			return true;
		}else{
			echo '<script type="text/javascript"> alert(\'Favor anexar Relatório de contagem de PF Estimada.\');</script>' ;
			return false;
		}
}
?>