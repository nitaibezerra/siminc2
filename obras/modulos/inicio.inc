<?php
ini_set("memory_limit", "2048M");

switch ( $_GET['carga'] ){
	case 'atualizaValorPrevisto':
		require_once APPRAIZ . "/www/obras/atualizaValorPrevisto.php";
		die;
}


/**
 * Página inicial do módulo Monitoramento de Obras
 * @author Fernando Bagno <fernandosilva@mec.gov.br>
 * @since 27/04/2010
 * @version 2.0
 * 
 */
if( $_POST['agrupamento'] ){
	$_SESSION["obras"]["filtros"]["agrupamento"] = $_POST['agrupamento'];
	$valida = '1';
}

//if( !$db->testa_superuser() && possuiPerfil( PERFIL_EMPRESA ) ){
//	header( "location:obras.php?modulo=principal/supervisao/rotas&acao=A" );
//	die;
//}

//ini_set("memory_limit", "2048M");
set_time_limit(0);

// Objetos da página
$obrInicio = new inicio();
$obras	   = new Obras();

if ($_REQUEST['titleFor']) {
	$sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.parnumseqob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
		FROM par.empenhoobra emo 
		INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
		INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
		INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
		WHERE emo.preid = '".$_REQUEST['titleFor']."' AND pag.pagstatus='A'
		UNION ALL
		SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.parnumseqob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
		FROM par.empenhoobrapar emo 
		INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
		INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
		INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
		WHERE emo.preid = '".$_REQUEST['titleFor']."' AND pag.pagstatus='A'";
	
	$pagamentoobra = $db->pegaLinha($sql);
	
	if($pagamentoobra) {
		echo "Pago<br>
			  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br> 
			  Nº da Ordem Bancária: ".$pagamentoobra['parnumseqob']."<br> 
			  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
			  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
			  ";	
	} else {
		echo "Não pago";	
	}
	
	die();
}

// Requisições diversas da tela
switch( $_REQUEST["requisicao"] ){
	case "atualizaarvore":
		$_SESSION["obrasarvore"]["arEntid"]       = $_REQUEST["arEntid"];	
		$_SESSION["obrasarvore"]["arEntidCampus"] = $_REQUEST["arEntidCampus"];	
		$_SESSION["obrasbkp"]["filtrosbkp"]       = $_SESSION["obras"]["filtros"]["filtros"];
		die;
	break;	
	
	case "filtrar":
		$_SESSION["obras"]["filtros"]["filtros"] = $obrInicio->filtraListaDeObras( $_REQUEST );	
	break;
	
	case "limpar":
		$_SESSION["obras"]["filtros"] = null;
	break;
	
	case "listafilho":
		$obrInicio->listaFilhos( $_REQUEST["entid"], $_SESSION["obras"]["filtros"]["filtros"] );
		die;	
	break;
	
	case "listaobra":
		$obrInicio->listaObras( $_REQUEST["entid"], $_SESSION["obras"]["filtros"]["filtros"], '', '', '', 'simples' );
		die;	
	break;

	case "listaobraSC":
		$obrInicio->listaObras( $_REQUEST["entid"], $_SESSION["obras"]["filtros"]["filtros"], 'abresc', '', '', 'simples' );
		die;	
	break;

	case "excluir":
		if ( true !== $stMensagem = $obras->DeletarObras($_REQUEST["obrid"]) ){
			alert( $stMensagem );
		}
	break;
	
	case "ordem":
		
		$_SESSION["obras"]["lista"] = "lista";
		
		switch( $_POST["ordem"] ){
			
			case 1 :
				$_SESSION["obras"]["ordem"] = "anexo";
			break;
			case 2 :
				$_SESSION["obras"]["ordem"] = "foto";
			break;
			case 3 :
				$_SESSION["obras"]["ordem"] = "restricoes";
			break;
			case 4 :
				$_SESSION["obras"]["ordem"] = "pi";
			break;
			case 5 :
				$_SESSION["obras"]["ordem"] = "aditivo";
			break;
			case 6 :
				$_SESSION["obras"]["ordem"] = "id";
			break;
			case 7 :
				$_SESSION["obras"]["ordem"] = "oi.numconvenio";
			break;
			case 8 :
				$_SESSION["obras"]["ordem"] = "nome";
			break;
			case 9 :
				$_SESSION["obras"]["ordem"] = "entdescricao";
			break;
			case 10 :
				$_SESSION["obras"]["ordem"] = "municipiouf";
			break;
			case 11 :
				$_SESSION["obras"]["ordem"] = "inicio";
			break;
			case 12 :
				$_SESSION["obras"]["ordem"] = "termino";
			break;
			case 13 :
				$_SESSION["obras"]["ordem"] = "situacao";
			break;
			case 14 :
				$_SESSION["obras"]["ordem"] = "atualizacao";
			break;
			case 15 :
				$_SESSION["obras"]["ordem"] = "executado";
			break;
			
		}
		die;
	break;
	
	case "xls":
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatorioObras".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatorioObras".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$obrInicio->listaObras( $_REQUEST["entid"], $_SESSION["obras"]["filtros"]["filtros"], '', '', '', 'xls' );
		exit;
	break;		
}

if( $_SESSION["obrasbkp"]["orgid"] ){
	
	$_SESSION["obras"]["filtros"] = $_SESSION["obrasbkp"]["filtrosbkp"];
	$_SESSION["obras"]["orgid"]   = $_SESSION["obrasbkp"]["orgid"];
	$_SESSION["obras"]["ordem"]   = $_SESSION["obrasbkp"]["ordem"];
	$_SESSION["obras"]["lista"]   = $_SESSION["obrasbkp"]["lista"];
	
	unset($_SESSION["obrasbkp"]);
	
	header( "location:obras.php?modulo=inicio&acao=A&orgid=".$_SESSION["obras"]["orgid"] );
	
}
//window.location='obras.php?modulo=inicio&acao=A&orgid=3'
// cria a sessão com o tipo de ensino selecionado
$_SESSION["obras"]["orgid"] = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obras"]["orgid"]; 
$_SESSION["obra"]["orgid"]  = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obra"]["orgid"]; 

if ( !possuiPerfil( PERFIL_ADMINISTRADOR ) ) {
	$dados = obrPegaOrgidPermitido( $_SESSION["usucpf"] );
	$i=0;
	if ( is_array($dados) && $dados[0] ){
		foreach( $dados as $orgao ){
			$orgid[$i] = $orgao['id'];
			$i++;
		}
		if ( !in_array($_SESSION["obras"]["orgid"],$orgid) ){
			$_SESSION["obras"]["orgid"] = $orgid[0];
			header( "location:obras.php?modulo=inicio&acao=A&orgid=".$orgid[0] );
			die;
		}
	}
}

// Cabeçalho padrão do SIMEC
include  APPRAIZ."includes/cabecalho.inc";


/*
 * Testa se abre o popup
 */
/*
if(!$db->testa_superuser()) {
	$sql = "select count(a.arqid) as c
			from obras.fotos f 
			inner join obras.supervisao s on s.supvid=f.supvid 
			inner join public.arquivo a on a.arqid=f.arqid 
			inner join seguranca.usuario u ON u.usucpf = a.usucpf 
			inner join obras.obrainfraestrutura o ON o.obrid = s.obrid 
			left join entidade.entidade ent ON ent.entid = o.entidunidade 
			inner join obras.situacaoobra so ON so.stoid = o.stoid 
			inner join entidade.endereco e ON e.endid = o.endid 
			inner join territorios.municipio m ON m.muncod = e.muncod 
			left join obras.tiposupervisao ts ON ts.tpsid = s.tpsid 
			where a.arqid/1000 between 647 and 725
			and a.arqid not in ( select distinct arqid from public.arquivo_recuperado)  
			and supstatus='A' and sisid=15 and obsstatus = 'A' and a.usucpf='".$_SESSION['usucpf']."'";
	$carq1 = $db->pegaUm($sql);
}

if(!$db->testa_superuser()) {
	$sql = "select count(a.arqid) as c
		 	from obras.fotos f 
			inner join obras.supervisao s on s.supvid=f.supvid 
			inner join public.arquivo a on a.arqid=f.arqid 
			inner join seguranca.usuario u ON u.usucpf = a.usucpf 
			inner join obras.obrainfraestrutura o ON o.obrid = s.obrid 
			left join entidade.entidade ent ON ent.entid = o.entidunidade 
			inner join obras.situacaoobra so ON so.stoid = o.stoid 
			inner join entidade.endereco e ON e.endid = o.endid 
			inner join territorios.municipio m ON m.muncod = e.muncod 
			left join obras.tiposupervisao ts ON ts.tpsid = s.tpsid 
			and a.arqid not in ( select distinct arqid from public.arquivo_recuperado)  
			and supstatus='A' and sisid=15  and obsstatus = 'A' and a.usucpf='".$_SESSION['usucpf']."'";
	$carq2 = $db->pegaUm($sql);
}
$carq = ($carq1 + $carq2);

if($carq > 0 ) {
	$texto = "<center>
				<img src=\"../imagens/alerta_sistema.gif\" />
			    <p><font size=3 color=red><b>Nota do Sistema!</b></font></p>
           	     <p style=\"font-weight:bold\" ><font size=3>Alguns arquivos anexados por você no SIMEC foram corrompidos,<br />
           	     para corrigir o problema você poderá enviá-los novamente.<br />
           	     Clique abaixo para ver a relação e proceder a correção.</p>
           	     <input type=\"button\" value=\"Ver a relação de arquivos\" style=\"cursor:pointer;font:16px Trebuchet Ms,Arial,Tahoma,Verdana,Helvetica,Sans-Serif;height:33px\" onclick=\"window.location='obras.php?modulo=sistema/public_arquivo/obras_arquivo&acao=A'\" />
          	  </center>";
	popupAlertaGeral($texto,"580px","260px");
}
*/
/*
 * FIM - Testa se abre o popup
 */
 
if( (obrPegaOrgidPermitido($_SESSION["usucpf"]) /*&& !possuiPerfil(PERFIL_SAA)*/) || $db->testa_superuser()){
	
	// Monta o título da dela no padrão SIMEC
	print "<br/>";
	print obrMontaAbasTipoEnsino( $_SESSION["usucpf"], $_REQUEST["orgid"] );
	monta_titulo( "Lista de Obras", "" );
	// cria a sessão com o tipo de ensino selecionado
	$_SESSION["obras"]["orgid"] = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obras"]["orgid"]; 
	$_SESSION["obra"]["orgid"]  = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obra"]["orgid"]; 
	
	if(1==2 && ($_SESSION["obras"]["orgid"] == ORGAO_SESU || $_SESSION["obras"]["orgid"] == ORGAO_SETEC )){
		$texto = "<div style=\"font-size:12px\" >Prezado Usuário,<br /><br />
		Informamos que o status de obra CONTRATO CANCELADO foi retirado do sistema. Todas as obras com status de CONTRATO CANCELADO foram migradas automaticamente para o status PARALISADA e o motivo da paralisação será CONTRATO RESCINDIDO.
		<br /><br />
		Em caso de dúvidas entrar em contato.
		</div>";

		popupAlertaGeral($texto,'450px',"200px");
	}	

	if ( $_SESSION["obras"]["orgid"] == ORGAO_FNDE ) {
		$texto = "<div style=\"font-size:12px\" >Prezado Usuário,<br /><br />
		Informamos que todas as obras foram migradas para o Obras 2.0 e devem ser atualizadas por lá.
		<br /><br />
		Em caso de dúvidas entrar em contato.
		</div>";

		popupAlertaGeral($texto,'450px',"200px");
	}
?>
	<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
	<script type="text/javascript" src="/includes/remedial.js"></script>
	<script type="text/javascript" src="/includes/superTitle.js"></script>
	<script type="text/javascript">
		var u='/obras/obras.php?modulo=inicio&acao=A&titleFor=';
	</script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	
	<form name="formulario" id="obrFormPesquisa" method="post" action="">
		<input type="hidden" name="requisicao" id="requisicao" value="filtrar"/>
		<input type="hidden" name="numero" value="" />
		<input type="hidden" name="arEntid"       id="arEntid" 	     value="<?=$_SESSION["obrasarvore"]["arEntid"] ?>" />
		<input type="hidden" name="arEntidCampus" id="arEntidCampus" value="<?=$_SESSION["obrasarvore"]["arEntidCampus"] ?>"/>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
			<?php
			if ( $_SESSION["obra"]["orgid"] == 3 ):
			?>
			<tr>
				<td colspan="2" class="" style="color: black; text-align: justify;"><font size=+2>
O Módulo Monitoramento de Obras do Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação (Simec) passou por reformulação a fim de melhorar os processos de monitoramento das ações para os usuários do sistema. 
Com isso todas as obras de educação básica foram migradas para o obras 2 e devem a partir de então serem acompanhadas nele (obras 2).
				</td>
			</tr>
			<?php
			die;
			endif;
			?>
			<tr>
				<td colspan="2" class="subTituloCentro">Argumentos de Pesquisa</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Agrupar por:</td>
				<td> <!--Se sessão da Página Ensino Básico for orgid = 3, checar radio button, Obras.  -->
					<?php 
					if( $valida <> '1' && !($_SESSION["obras"]["filtros"]["agrupamento"]) || $_POST['agrupamento'] == 'O' || $_SESSION["obras"]["filtros"]["agrupamento"] == 'O' ){
						$chkObras = 'checked="checked"';
						$chkUnidades = '';
					}
					/*Se sessão da Página Ensino Superior for orgid = 1	ou Ensino Profissional for orgid = 2, checar radio button, Unidade.*/
					elseif( $valida <> '1' || $_POST['agrupamento'] == 'U' || $_SESSION["obras"]["filtros"]["agrupamento"] == 'U' ){
						$chkUnidades = 'checked="checked"';
						$chkObras = '';
					}
//					$chkObras = 'checked="checked"';
//					$chkUnidades = '';
					?> 
					<input type="radio" name="agrupamento" id="agrupamentoU" value="U"  <?php echo $chkUnidades; ?>/> Unidades
					<input type="radio" name="agrupamento" id="agrupamentoO" value="O"  <?php echo $chkObras; ?>/> Obras
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
				<td>
					<?php 
					
						$tobaid = $_SESSION["obras"]["filtros"]["tobaid"];
						$sql = "SELECT 
									tobaid as codigo, 
									tobadesc as descricao 
								FROM 
									obras.tipoobra 
								ORDER BY 
									tobadesc";
					
						$db->monta_combo( "tobaid", $sql, "S", "Todos", "", "", "", "", "N", "tobaid" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Situação da Obra:</td>
				<td>
					<?php 
					
						$stoid = $_SESSION["obras"]["filtros"]["stoid"];
						
						if($_SESSION['obras']['orgid'] == ORGAO_SESU){
							$stWhere .= " and stoedsuperior = true";	
						}
						elseif($_SESSION['obras']['orgid'] == ORGAO_SETEC){
							$stWhere .= " and stoedprofissional = true";	
						}
						elseif($_SESSION['obras']['orgid'] == ORGAO_FNDE){
							$stWhere .= " and stoedbasica = true";	
						}
						elseif($_SESSION['obras']['orgid'] == ORGAO_ADM){
							$stWhere .= " and stoedadm = true";	
						}
						elseif($_SESSION['obras']['orgid'] == ORGAO_REHUF){
							$stWhere .= " and stoedhospitais = true";	
						}
						elseif($_SESSION['obras']['orgid'] == ORGAO_MILITAR){
							$stWhere .= " and stoedmilitares = true";	
						}
						
						$sql = "SELECT 
									stoid as codigo, 
									stodesc as descricao 
								FROM 
									obras.situacaoobra
								WHERE
									stostatus = 'A'
								{$stWhere} 
								ORDER BY 
									stodesc";
					
						$db->monta_combo( "stoid", $sql, "S", "Todas", "", "", "", "", "N", "stoid" );
						
					?>
				</td>
			</tr>
            <!--
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
				<td>
					<?php 
					
						$cloid = $_SESSION["obras"]["filtros"]["cloid"];
					
						$sql = "SELECT 
									cloid as codigo,
									clodsc as descricao
								FROM 
								  	obras.classificacaoobra
								ORDER BY
									clodsc";
					
						$db->monta_combo( "cloid", $sql, "S", "Todas", "", "", "", "", "N", "cloid" );
						
					?>
				</td>
			</tr>
			-->
			<?php if($_SESSION["obra"]["orgid"] != 2): ?>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
					<td>
						<?php 
						
							$tpoid = $_SESSION["obras"]["filtros"]["tpoid"];
						
							$sql = "SELECT 
										tpoid AS codigo, 
										tpodsc AS descricao
	  								FROM 
	  									obras.tipologiaobra
									WHERE
	  									tpostatus = 'A'
									AND
										orgid = {$_SESSION["obra"]["orgid"]}
									ORDER BY
										tpodsc";
						
							$db->monta_combo( "tpoid", $sql, "S", "Todas", "", "", "", "", "N", "tpoid" );
							
						?>
					</td>
				</tr>
			<?php endif; ?>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Programa:</td>
				<td>
					<?php 
					
						$prfid = $_SESSION["obras"]["filtros"]["prfid"];
						
						$sql = "SELECT 
									prfid as codigo,
									prfdesc as descricao
							  	FROM 
							  		obras.programafonte
							  	WHERE
							  		orgid = {$_SESSION["obras"]["orgid"]}
							  	ORDER BY
							  		prfdesc";
					
						$db->monta_combo( "prfid", $sql, "S", "Todos", "", "", "", "", "N", "prfid" );
						
					?>
				</td>
			</tr>
			<?php if($_SESSION["obra"]["orgid"] != 1 && $_SESSION["obra"]["orgid"] != 2): ?>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Fonte:</td>
					<td>
						<?php 
						
							$tooid = $_SESSION["obras"]["filtros"]["tooid"];
							
							$sql = "SELECT 
										tooid AS codigo, 
										toodescricao AS descricao
	  								FROM 
	  									obras.tipoorigemobra
	  								WHERE
	  									toostatus = 'A'
									UNION ALL
									SELECT
										9999 as codigo,
										'Não Informado' as descricao
									ORDER BY
										descricao";
						
							$db->monta_combo( "tooid", $sql, "S", "Todos", "", "", "", "", "N", "tooid" );
							
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Modalidade de Ensino:</td>
					<td>
						<?php 
						
							$moeid = $_SESSION["obras"]["filtros"]["moeid"];
							
							$sql = "SELECT 
										moeid AS codigo, 
										moedsc AS descricao
	  								FROM 
	  									obras.modalidadeensino
									WHERE
										moestatus = 'A'
									ORDER BY
										moedsc";
						
							$db->monta_combo( "moeid", $sql, "S", "Todos", "", "", "", "", "N", "moeid" );
							
						?>
					</td>
				</tr>
			<?php endif; ?>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
				<td>
					<?php 
					
						$entidunidade = $_SESSION["obras"]["filtros"]["entidunidade"];
						
						if($_SESSION["obras"]["orgid"]==5){
							$sql = "SELECT 
										ent.entid as codigo, 
										upper(ena.entsig) || ' - ' || upper(ent.entnome) as descricao
									FROM
										entidade.entidade ent
									INNER JOIN 
										obras.obrainfraestrutura oi ON oi.entidunidade = ent.entid
									INNER JOIN
										entidade.funcaoentidade fen ON fen.entid = ent.entid 
									INNER JOIN
										entidade.funcao fun ON fun.funid = fen.funid
									INNER JOIN
										entidade.funentassoc fue ON fue.fueid = fen.fueid
									INNER JOIN
										entidade.entidade ena ON ena.entid = fue.entid 
									WHERE
										orgid = {$_SESSION["obras"]["orgid"]} AND
										obsstatus = 'A'
									GROUP BY 
										codigo, 
										descricao 
									ORDER BY 
										descricao";
						}else{
							$sql = "SELECT 
										ee.entid as codigo, 
										upper(ee.entnome) as descricao 
									FROM
										entidade.entidade ee
									INNER JOIN 
										obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid 
									WHERE
										orgid = {$_SESSION["obras"]["orgid"]} AND
										obsstatus = 'A'
									GROUP BY 
										ee.entnome, 
										ee.entid 
									ORDER BY 
										ee.entnome";
						
						}
						//ver($sql,d);
						$db->monta_combo( "entidunidade", $sql, "S", "Todos", "", "", "", "", "N", "entidunidade" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Última atualização:</td>
				<td>
					<?php 
					
						$ultatualizacao = $_SESSION["obras"]["filtros"]["ultatualizacao"];
						
						$arSel = array(
										array("codigo"    => 1,
										      "descricao" => "Em até 45 dias"),
										array("codigo"    => 2,
										      "descricao" => "Entre 45 e 60 dias"),
										array("codigo"    => 3,
										      "descricao" => "Mais de 60 dias"),
									  );
					
						$db->monta_combo("ultatualizacao", $arSel, "S", "Todos", "", "", "", "", "N", "ultatualizacao" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Nome da Obra / Nº do Convênio / Nº do PI / ID:</td>
				<td>
					<?php $obrtextobusca = $_SESSION["obras"]["filtros"]["obrtextobusca"]; ?>
					<?php print campo_texto( 'obrtextobusca', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">UF:</td>
				<td>
					<?php 
					
						$estuf = $_SESSION["obras"]["filtros"]["estuf"];
						
						$sql = "SELECT
									estuf as codigo,
									estdescricao as descricao
								FROM
									territorios.estado
								ORDER BY
									estdescricao";
						
						$db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Município:</td>
				<td>
					<?php $mundsc = $_SESSION["obras"]["filtros"]["mundsc"]; ?>
					<?php print campo_texto( 'mundsc', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Esfera:</td>
				<td>
					<?php 					
						$obrtipoesfera = $_SESSION["obras"]["filtros"]["obrtipoesfera"];
						
						$arrEsfera = array(
										array('codigo' => 'E', 'descricao' => 'Estadual'),
										array('codigo' => 'M', 'descricao' => 'Municipal')
									);
						$db->monta_combo( "obrtipoesfera", $arrEsfera, "S", "Todas", "", "", "", "", "N", "obrtipoesfera" );						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
				<td>
					<input type="radio" name="foto" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="foto" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="foto" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "" ){ print "checked='checked'"; } ?> /> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui vistoria:</td>
				<td>
					<input type="radio" name="vistoria" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="vistoria" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="vistoria" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui restrição:</td>
				<td>
					<input type="radio" name="restricao" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="restricao" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="restricao" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui PI:</td>
				<td>
					<input type="radio" name="planointerno" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="planointerno" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="planointerno" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui Aditivo:</td>
				<td>
					<input type="radio" name="aditivo" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="aditivo" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="aditivo" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui Supervisão:</td>
				<td>
					<input type="radio" name="supervisao" onclick="exibeSubFiltros('S')" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="supervisao" onclick="exibeSubFiltros('N')" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="supervisao" onclick="exibeSubFiltros('N')" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "" ){ print "checked='checked'"; } ?>/> Todas
					<script>
						function verificaSubFiltro(tipo)
						{
							if(tipo == "entre"){
								document.getElementById('fim_subfiltro').style.display = "";
							}else{
								document.getElementById('fim_subfiltro').style.display = "none";
							}
						}
						
						function exibeSubFiltros(tipo)
						{
							if(tipo == "S"){
								document.getElementById('inicio_subfiltro').style.display = "";
							}else{
								document.getElementById('inicio_subfiltro').style.display = "none";
							}
						}
					</script>
					<span style="padding-left:10px;display:<?php echo $_SESSION["obras"]["filtros"]["supervisao"] == "S" ? "" : "none" ?>" id="inicio_subfiltro"  >
						<?php $arrTipoSupervisao = array( 
															0=> array("codigo" => ">", "descricao" => "Maior que"),
															1=> array("codigo" => ">=", "descricao" => "Maior Igual a"),
															2=> array("codigo" => "=", "descricao" => "Igual a"),
															3=> array("codigo" => "<", "descricao" => "Menor que"),
															4=> array("codigo" => "<=", "descricao" => "Menor Igual a"),
															5=> array("codigo" => "entre", "descricao" => "Entre")
														)?>
						<?php $tiposupervisao = $_SESSION["obras"]["filtros"]["tiposupervisao"]; $db->monta_combo("tiposupervisao",$arrTipoSupervisao,"S","","verificaSubFiltro","") ?>
						<?php $subfiltro_inicio = $_SESSION["obras"]["filtros"]["subfiltro_inicio"]; echo campo_texto("subfiltro_inicio","N","S","",10,20,"[#]","","","","","id='subfiltro_inicio'") ?>
						<span id="fim_subfiltro" style="display:<?php echo $_SESSION["obras"]["filtros"]['tiposupervisao'] && $_SESSION["obras"]["filtros"]['tiposupervisao'] == "entre" ? "" : "none" ?>"  >
							de 
							<?php $subfiltro_fim = $_SESSION["obras"]["filtros"]["subfiltro_fim"]; echo campo_texto("subfiltro_fim","N","S","",10,20,"[#]","","","","","id='subfiltro_fim'") ?>
						</span>
					</span>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Mobiliário:</td>
				<td>
					<input type="radio" name="mobiliario" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["mobiliario"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="mobiliario" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["mobiliario"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="mobiliario" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["mobiliario"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<?if($_SESSION["obra"]["orgid"] == 3){ ?>
			<tr id="vistoriainseridaunidade">
				<td class="SubTituloDireita">Tipo Vistoria (Realizado por):</td>
				<td>
					<?
					$rsuid = $_SESSION["obras"]["filtros"]["rsuid"];
					
					$sql = "SELECT rsuid as codigo, rsudsc as descricao
							FROM  obras.realizacaosupervisao";
					//$db->monta_combo("rsuid", $sql, "S", "Todas", "", "", "", "", "N", "rsuid" ); 
					$arrSupervisao = $db->carregar( $sql );
					$arrSupervisao = $arrSupervisao ? $arrSupervisao : array();
					
					$html = '<select name="rsuid" class="CampoEstilo" style="width: auto" id="rsuid">
								<option value="">Todas</option>
								<option value="0" '.($rsuid == '0' ? 'selected="selected"' : '' ).'>Sem Supervisão</option>';
					foreach ($arrSupervisao as $v) {
						$select =  '';
						if( $rsuid == $v['codigo'] ) $select = 'selected="selected"';
						$html .= '<option value="'.$v['codigo'].'" '.$select.'>'.$v['descricao'].'</option>';	
					}
					$html.= '</select>';
					
					echo $html;
					?>
				</td>
			</tr>
			<?} ?>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Valor da Obra:</td>
				<td>
					De:&nbsp;
					<?php $vlrmenor = $_SESSION["obras"]["filtros"]["vlrmenor"]; echo campo_texto( 'vlrmenor', 'N', 'S', '', 11, 30, '[#].##', '', 'left', '', 0, '');?>
					Até:&nbsp;
					<?php $vlrmaior = $_SESSION["obras"]["filtros"]["vlrmaior"]; echo campo_texto( 'vlrmaior', 'N', 'S', '', 11, 30, '[#].##', '', 'left', '', 0, '');?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Nível de Preenchimento:</td>
				<td>
					<?php 
					
						$nivelpreenchimento = $_SESSION["obras"]["filtros"]["nivelpreenchimento"];
						
						$arrNivel = array(
										array("codigo"    => 1,
										      "descricao" => "Amarelo ( Obras atualizadas entre 45 e 60 dias )"),
										array("codigo"    => 2,
										      "descricao" => "Azul ( Obras concluídas )"),
										array("codigo"    => 3,
										      "descricao" => "Verde ( Obras atualizadas há menos de 45 dias atrás )"),
										array("codigo"    => 4,
										      "descricao" => "Vermelho ( Obras atualizadas há mais de 60 dias )"),
									  );
					
						$db->monta_combo("nivelpreenchimento", $arrNivel, "S", "Todos", "", "", "", "", "N", "nivelpreenchimento" );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">% Executado da Obra:</td>
				<td>
					<table>
						<tr>
							<th>Mínimo</th>
							<th>Máximo</th>
						</tr>
						<tr>
							<?php
								
								for ($i = 0; $i <= 100; $i++ ){
									$arPercentual[] = array( 'codigo' =>  "$i" , 'descricao' => "$i%" );
								}
								/*
								$arPercentual[] = array( 'codigo' =>  0 , 'descricao' => '0 %' );
								$arPercentual[] = array( 'codigo' =>  5 , 'descricao' => '5 %' );
								$arPercentual[] = array( 'codigo' => 10 , 'descricao' => '10 %' );
								$arPercentual[] = array( 'codigo' => 15 , 'descricao' => '15 %' );
								$arPercentual[] = array( 'codigo' => 20 , 'descricao' => '20 %' );
								$arPercentual[] = array( 'codigo' => 25 , 'descricao' => '25 %' );
								$arPercentual[] = array( 'codigo' => 30 , 'descricao' => '30 %' );
								$arPercentual[] = array( 'codigo' => 35 , 'descricao' => '35 %' );
								$arPercentual[] = array( 'codigo' => 40 , 'descricao' => '40 %' );
								$arPercentual[] = array( 'codigo' => 45 , 'descricao' => '45 %' );
								$arPercentual[] = array( 'codigo' => 50 , 'descricao' => '50 %' );
								$arPercentual[] = array( 'codigo' => 55 , 'descricao' => '55 %' );
								$arPercentual[] = array( 'codigo' => 60 , 'descricao' => '60 %' );
								$arPercentual[] = array( 'codigo' => 65 , 'descricao' => '65 %' );
								$arPercentual[] = array( 'codigo' => 70 , 'descricao' => '70 %' );
								$arPercentual[] = array( 'codigo' => 75 , 'descricao' => '75 %' );
								$arPercentual[] = array( 'codigo' => 80 , 'descricao' => '80 %' );
								$arPercentual[] = array( 'codigo' => 85 , 'descricao' => '85 %' );
								$arPercentual[] = array( 'codigo' => 90 , 'descricao' => '90 %' );
								$arPercentual[] = array( 'codigo' => 95 , 'descricao' => '95 %' );
								$arPercentual[] = array( 'codigo' => 100 , 'descricao' => '100 %' );
								*/
								$percentualinicial = $_SESSION["obras"]["filtros"]['percentualinicial'];
								$percentualfinal   = $_SESSION["obras"]["filtros"]['percentualfinal'];
								
								$percfinal = $percentualfinal == '' ? 100 : $percentualfinal; 
								//$percinicial = ($percentualinicial == '' && $_SESSION["obra"]["orgid"] == 3) ? 90 : $percentualinicial; 
								
								print '<td>';
								$db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
								print '</td><td>';
								$db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false,$percfinal);
								print '</td>';
								
							?>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td style="width: 190px;"></td>
				<td>
					<input type="button" name="obrBtPesquisaInicio" value="Pesquisar" onclick="obrFiltraLista();" style="cursor: pointer;"/>
					<input type="button" name="obrBtVerTudoInicio" value="Ver Todas" onclick="obrVerTodas();" style="cursor: pointer;"/>
					<input type="button" name="obrBtVerTudoInicio" value="Exportar XLS" onclick="obrVerXls();" style="cursor: pointer;"/>
				</td>
			</tr>
		</table>
	</form>
		<?php 
		//fazendo o include da classe
		require_once APPRAIZ . "includes/classes/Atualizacao.class.inc";
		
//		$mensagem = '<p align="center" style="font-size: 15px;"><font size="4" color="red"><b>Atenção!</b></font><br><br>Existem novas atualizações.<br><br>Para visualizar as atualizações recentes clique no ícone <br><br><table><tbody><tr><td align="right"><img border="0" align="right" src="../includes/layout/montanhas/img/bt_help.png" title="Atualizações" alt="Atualizações"></td><td align="left"><font color="#dd0000"><strong>Atualizações</strong></font></td></tr></tbody></table></p>';
		$Atualizacao = new Atualizacao("float: right; position: absolute; top: 153px; right: 75px;");
	?>
	
	<?php 
	//$arPerfilIncluiObra = $_SESSION["obras"]["orgid"] == ORGAO_FNDE ? array(PERFIL_ADMINISTRADOR) : array( PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_SUPERVISORMEC, PERFIL_GESTORUNIDADE );
	$arPerfilIncluiObra = $_SESSION["obras"]["orgid"] == ORGAO_FNDE ? array(PERFIL_SUPERUSUARIO) : array( PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_SUPERVISORMEC, PERFIL_GESTORUNIDADE );
	$permissaoIncluiObra = possuiPerfil( $arPerfilIncluiObra );
	
	//if( $permissaoIncluiObra && $_SESSION['obras']['orgid'] <> 3 ): 
	if( $permissaoIncluiObra ):
	?>
	<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
		<tr>
			<td style="font-weight: bold;">
				<a style="cursor: pointer;" onclick="obrIrParaCaminho( '', 'novaobra', <?php print $_SESSION["obras"]["orgid"]; ?> );" title="Clique para incluir uma nova obra no sistema">
					<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> Incluir nova obra
				</a>
			</td>
		</tr>
	</table>
	<?php endif; ?>
	
	<?php
		// Valida a listas cadastradas no sistema
		//Se Ensino Básico com orgid = 3 e valida != 1, listar Obras.  
		if( $valida <> '1' && !($_SESSION["obras"]["filtros"]["agrupamento"]) ){
			$_SESSION["obras"]["filtros"]["agrupamento"] = 'O';
		//Senão se Ensino Superior com orgid = 1 ou Ensino Profissional com orgid = 2 e valida != 1, listar Unidades.  
		}
//		elseif($_SESSION['obras']['orgid'] <> 3 && $valida <> '1'){
//			$_SESSION["obras"]["filtros"]["agrupamento"] = 'U';
//		}
//		$_SESSION["obras"]["filtros"]["agrupamento"] = 'O';
		// Monta a lista de obras cadastradas no sistema 
//		ver($_SESSION["obras"]["filtros"]["filtros"]);
        if(isset($_REQUEST['stoid'])){
		    $obrInicio->listaDeObras( $_SESSION["obras"]["filtros"]["filtros"] );
        }
	?>
	
	<?php if( $permissaoIncluiObra && $_SESSION['obras']['orgid'] <> 3 ): ?>
	<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
		<tr>
			<td style="font-weight: bold;">
				<a style="cursor: pointer;" onclick="obrIrParaCaminho( '', 'novaobra', <?php print $_SESSION["obras"]["orgid"]; ?> );" title="Clique para incluir uma nova obra no sistema">
					<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> Incluir nova obra
				</a>
			</td>
		</tr>
	</table>
	<?php endif; ?>
<?php }else{ ?>

	<?php print "<br/>"; ?>
	<?php monta_titulo( "Monitoramento de Obras", "" ); ?>
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr style="text-align: center; color: #ff0000;">
			<td>
				Usuário sem permissões para visualizar obras
			</td>
		</tr>
	</table>
<?php } ?>
