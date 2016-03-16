<?PHP

//SELECIONA evento
if( $_POST['ajaxestuf'] )
{
	header('content-type: text/html; charset=ISO-8859-1');

	$sql = "select
			 muncod as codigo, mundescricao as descricao
			from
			 territorios.municipio
			where
			 estuf = '".$_POST['ajaxestuf']."'
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', 255, '', 'muncod' ));
}

if( $_POST['toolTypeAjax'] != ''){
	$sql = "SELECT distinct
                       aqb.eveid,
                        arq.arqid ,
                        tarq.tpadescricao || ' - ' ||  arq.arqtamanho || ' kbs' as title ,
                        CASE WHEN aval.aevid IS NOT NULL   THEN 'Avaliação preenchida'
                        ELSE ''
                        END AS avaliacao
                    FROM
                        ((public.arquivo arq INNER JOIN evento.anexoevento aqb
                        ON arq.arqid = aqb.arqid) INNER JOIN evento.tipoanexo tarq
                        ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
                        ON usu.usucpf = arq.usucpf
                        LEFT JOIN evento.avaliacaoevento AS aval ON aval.eveid = aqb.eveid
                    WHERE
                        arq.arqstatus = 'A' AND  aqb.eveid = ".$_POST['toolTypeAjax'];

	$rsDadosAnexo = $db->carregar( $sql );
	for( $i =0; $i< count($rsDadosAnexo); $i++){
		if( $rsDadosAnexo[0]['avaliacao'] != ''){
			$avaliacaoText = $rsDadosAnexo[0]['avaliacao'];
		}
		$content .= $rsDadosAnexo[$i]['title'].'<br>';
	}
	$content.=$avaliacaoText;
	echo $content;
	die();
}

if ( $_REQUEST['selecionar'] ) {
	# TODO: verificar se o evento existe
	$_SESSION['evento']['eveid'] = (integer) $_REQUEST['selecionar'];

	echo '<script>
			location.href="evento.php?modulo=principal/cadEvento&acao=A";
		  </script>';

	die();

	//header("Location: ?modulo=principal/cadEvento&acao=A");
	//redirecionar( 'principal/atividade_/arvore', 'A' );
}

if ( $_REQUEST['selecionarProcesso'] ) {
	# TODO: verificar se o evento existe
	$_SESSION['copid'] = (integer) $_REQUEST['selecionarProcesso'];

	header("Location: ?modulo=principal/cadProcesso&acao=A");
	//redirecionar( 'principal/atividade_/arvore', 'A' );
}

if( $_POST['ajaxsession'] != ''){
	$_SESSION['evento']['eveid'] = $_POST['ajaxsession'];
	die();
}

if( $_REQUEST['evento'] == 'editar')
{
	$_SESSION['evento']['eveid'] = $rsDadosEvento[$i]['eveid'];
}

/*
if( possuiPerfil(Array(786)) && $_SESSION['sisbaselogin'] == 'simec_desenvolvimento_old' ){
	header("Location: evento.php?modulo=principal/faturaPassagens&acao=A");
}
*/



# Array de perfis que veem dois módulos
$arPerfisVerModulos = array(EVENTO_PERFIL_CGCC,
					 	    EVENTO_PERFIL_CONSULTA,
					 	    EVENTO_PERFIL_SUPER_USUARIO
						    );

# Array de perfis que veem Eventos
$arPerfisEvento = array(EVENTO_PERFIL_CADASTRADOR,
				 	    EVENTO_PERFIL_SAA,
				 	    EVENTO_PERFIL_SEC_EXECUTIVA_EVENTOS,
				 	    EVENTO_PERFIL_PERFIL_EMPRESA,
				 	    EVENTO_PERFIL_SAA_FINANCEIRO
					    );

# Array de perfis que veem Compras
$arPerfisCompras = array(EVENTO_PERFIL_ORDENADOR_DESPESA_COMPRAS,
					 	 EVENTO_PERFIL_DEMANDANTE_COMPRAS,
					 	 EVENTO_PERFIL_CONSULTA_COMPRAS
						);

# Array de perfis que veem Compras
$arPerfisSolicitacaoAjudaCusto = array(EVENTO_PERFIL_SOLICITADOR,
					 	 EVENTO_PERFIL_VALIDADOR,
					 	 EVENTO_PERFIL_AGENTE_FINANCEIRO,
					 	 EVENTO_PERFIL_AUTORIZADOR,
					 	 EVENTO_PERFIL_ADMINISTRADOR_DIARIAS
						);

$perfis = arrayPerfil();

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
//if( ( ( ( in_array(EVENTO_PERFIL_SEC_EXECUTIVA_EVENTOS, $perfis ) ) || in_array(PERFIL_SAA, $perfis ) ) || ( in_array(PERFIL_SUPER_USUARIO, $perfis ) )) && (!$_GET['submod'])){
if(possuiPerfil($arPerfisVerModulos) || (possuiPerfil($arPerfisEvento) && possuiPerfil($arPerfisCompras)) ){
	$titulo_modulo = "Eventos / Compras";
	monta_titulo( $titulo_modulo, 'Selecione o Sub-Módulo que deseja visualizar.' );

	?>
	<script> function sisManutencao(){ alert("Módulo em manutenção"); }</script>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<th width ="50%">
				<a style="cursor:pointer;" onclick="window.location.href='evento.php?modulo=principal/inicioEvento&acao=A'">Eventos</a>
			</th>
			<th>
				<!-- a style="cursor:pointer;" onclick="window.location.href='evento.php?modulo=inicio&acao=C&submod=compra'">Compras</a-->
				<a style="cursor:pointer;" onclick="window.location.href='evento.php?modulo=principal/inicioCompraUnidade&acao=A'">Compras</a>
			</th>
		</tr>
		<tr>
			<td valign="top">
						<?$group = " group by est.esddsc, est.esdid order by est.esdid" ;
				    $sql = sprintf(" SELECT
				                         CASE
				                          WHEN est.esddsc IS NOT NULL THEN est.esddsc
				                          ELSE 'Aguardando iniciação'
				                         END AS esddsc,
				                         est.esdid,
				                          count(*) as count
				                     FROM
				                         evento.evento AS ev
				                    LEFT JOIN workflow.documento d ON d.docid = ev.docid
				                    LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid
									WHERE est.esdid IS NOT NULL
									AND ev.evestatus = 'A'
				                    %s",
				                    $group);
					$sql2 = "select
								estdescricao as descricao,
								ed.esdid as esdid,
								est.estuf as estuf,
								count(*) as count
							from
								evento.evento ev
							LEFT join
								territorios.estado est ON upper(ev.estuf) = upper(est.estuf)
							INNER join
								workflow.documento d on d.docid=ev.docid
							INNER join
								workflow.estadodocumento ed on ed.esdid=d.esdid
							where
								d.esdid = |agrupador|
								AND ev.evestatus = 'A'
							group by
								ed.esdid,
								est.estuf,
								descricao
							order by
								descricao";
					$sqlAgrupador = array("sql" => $sql2, "agrupador" => "esdid", "link" => "evento.php?modulo=principal/inicioEvento&acao=A&submod=evento", "campo" => array("esddsc","estdescricao"), "get" => array("esdid","estuf"), "arrOff" => array("estuf"), "exibeLink" => array("descricao"));
				   	$link = array("link" => "evento.php?modulo=principal/inicioEvento&acao=A&submod=evento", "campo" => "esddsc", "get" => "esdid");
					listaSituacaoPorUF('tabela_1',$sql,'',$cabecalho,$sqlAgrupador,"S",$link);
				   ?>

			</td>
			<td valign="top">
					<?$group = " group by est.esddsc, est.esdid order by est.esdid" ;
				    $sql = sprintf("SELECT  est.esddsc as estado,
											est.esdid as esdid,
        									count(*) as count
									FROM evento.coadesao AS coa
   									     LEFT JOIN evento.coprocesso cop ON coa.copid = cop.copid
										 LEFT JOIN workflow.documento d ON d.docid = coa.docid
										 LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid
     							    WHERE est.esdid IS NOT NULL
   									%s",
				                    $group);
					$sql2 = "select    usg.usgdsc as uasg,
									   usg.usgid as usgid,
									   cp.copid as copid,
									   cp.copdsc as processo,
									   ed.esdid as esdid
							from 	evento.coadesao AS coa
							INNER join evento.uasg usg on coa.usgid = usg.usgid
							INNER join evento.coprocesso cp on coa.copid = cp.copid
							INNER join workflow.documento d on d.docid=coa.docid
							INNER join workflow.estadodocumento ed on ed.esdid=d.esdid
							WHERE 	d.esdid = |agrupador|
							group by usg.usgdsc, usg.usgid, cp.copid, cp.copdsc, ed.esdid, ed.esddsc
							order by esdid, usgdsc;";
					$sqlAgrupador = array("sql" => $sql2, "agrupador" => "esdid", "link" => "evento.php?modulo=principal/cadProcesso&acao=A", "campo" => array("copdsc"), "get" => array("copid"), "arrOff" => array("copid"), "exibeLink" => array("processo"));
				 	$link = array("link" => "evento.php?modulo=principal/cadProcesso&acao=A", "campo" => "copdsc", "get" => "copid");
					listaSituacaoPorUF('tabela_2',$sql,'',$cabecalho,$sqlAgrupador,"S",$link);
				   ?>
			</td>
		</tr>
	</table>

	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<th width ="50%">
				<a style="cursor:pointer;" onclick="window.location.href='evento.php?modulo=principal/inicioContrato&acao=A'">Contratos</a>
			</th>
			<th>

			</th>
		</tr>
		<tr>
			<td valign="top">
						<?$group = " group by id, descricao order by descricao" ;
				    $sql = sprintf(" SELECT tpc.tpcid as id,
				    					    tpc.tpcdsc as descricao,
				    					    COUNT(*) AS COUNT
  									 FROM evento.cttipocontrato tpc
									 INNER JOIN evento.ctcontrato ctr on ctr.tpcid = tpc.tpcid
									 INNER JOIN entidade.entidade ent on ent.entid = ctr.entidcontratada
				                    %s",
				                    $group);
					$sql2 = "SELECT ent.entnome as contratada,
									ent.entid as entid,
									tpc.tpcdsc as tpcdsc,
									tpc.tpcid as tpcid
  							 FROM evento.cttipocontrato tpc
							 INNER JOIN evento.ctcontrato ctr on ctr.tpcid = tpc.tpcid
						  	 INNER JOIN entidade.entidade ent on ent.entid = ctr.entidcontratada
							order by tpcdsc, contratada";
					$sqlAgrupador = array("sql" => $sql2, "agrupador" => "tpcid", "link" => "evento.php?modulo=inicio&acao=C&submod=evento", "campo" => array("tpcdsc","entnome"), "get" => array("tpcid","entid"), "arrOff" => array("entid"), "exibeLink" => array("contratada"));
				   	$link = array("link" => "evento.php?modulo=inicio&acao=C&submod=evento", "campo" => "tpcdsc", "get" => "tpcid");
					listaSituacaoPorUF('tabela_3',$sql,'',$cabecalho,$sqlAgrupador,"S",$link);
				   ?>

			</td>
			<td valign="top">
			</td>
		</tr>
	</table>

<?php
}elseif(possuiPerfil($arPerfisEvento) && !possuiPerfil($arPerfisCompras)){
?>
	<script>
		window.location.href = 'evento.php?modulo=principal/inicioEvento&acao=A';
	</script>
<?php
}elseif(possuiPerfil($arPerfisCompras) && !possuiPerfil($arPerfisEvento)){
?>
	<script>
		window.location.href = 'evento.php?modulo=principal/inicioCompraUnidade&acao=A';
	</script>
<?php
}elseif(possuiPerfil($arPerfisSolicitacaoAjudaCusto)){
?>
	<script>
		window.location.href = 'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A';
	</script>
<?php
}
?>