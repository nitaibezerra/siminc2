<?php
$sbaid = $_REQUEST['sbaid'];


if( $_REQUEST['sbaid'] ){

	
function retornaEscolaPar($entid)
{	
	global $db;
	$sql = "select escid from par.escolas where entid = $entid";
	$escid = $db->pegaUm($sql);
	if($escid){
		return $escid;
	}else{
		$escid = $db->pegaUm("insert into par.escolas (entid) values ($entid) returning escid");
		$db->commit();
		return $escid;	
	}
}

function salvarEscolas()
{
	$oSes = new SubacaoEscola();
	$oSes->deletaEscolasPorSubacao($_GET['sbaid'],$_GET['ano']);
	
	$n = 0;

	if(is_Array($_POST['escola'])){
		foreach($_POST['escola'] as $entid => $esc){
	//		if($entid && ( $_POST['sesquantidade'][$n])){
			if($entid && is_numeric($entid)){
				$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$entid);
				$oSes = new SubacaoEscola($sesid);
				$oSes->escid 		 = retornaEscolaPar($entid);
			//	$oSes->sesquantidade = $_POST['sesquantidade'][$n] ? str_replace(".","",$_POST['sesquantidade'][$n]) : 0;
				$oSes->sesano 		 = $_GET['ano'];
				$oSes->sesstatus	 = "A";
				$oSes->sbaid 		 = $_GET['sbaid'];
				$oSes->salvar();
				$oSes->commit();
				$n++;
			}
	
		}
	}
	
	if($n == 0){
		echo "<script>
				alert('Não houve modificações!');
				window.close();
			</script>";
	}else{
		echo "<script>
				alert('Operação realizada com sucesso!');
				parent.window.opener.location.href = 'par.php?modulo=principal/subacao&acao=A&sbaid={$_GET['sbaid']}&anoatual={$_GET['ano']}';
				window.close();
			</script>";	
	}
	
	exit;
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}

if( $_REQUEST['inserirEscola'] ){

	global $db;
	
	$oSes = new SubacaoEscola();
	
	if( $_REQUEST['checado'] == 'true' ){
		
		if( $_REQUEST['entid'] == 'todos' ){

			if( $_REQUEST['escolas'] ){
			
			$arrEntid = explode(",",$_REQUEST['escolas']);
			
				foreach( $arrEntid as $entid ){
					if(!empty($entid)&&is_numeric($entid)){				
						$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$entid);
						
						$oSes = new SubacaoEscola($sesid);
						$oSes->escid 		 = retornaEscolaPar($entid);
						$oSes->sesano 		 = $_GET['ano'];
						$oSes->sesstatus	 = "A";
						$oSes->sbaid 		 = $_GET['sbaid'];
						$oSes->salvar();
						$oSes->commit();
					}
				}
			}
			
		} else {
						
			if($_REQUEST['entid']){	
				$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$_REQUEST['entid']);
				
				$oSes = new SubacaoEscola($sesid);
				$oSes->escid 		 = retornaEscolaPar($_REQUEST['entid']);
				$oSes->sesano 		 = $_GET['ano'];
				$oSes->sesstatus	 = "A";
				$oSes->sbaid 		 = $_GET['sbaid'];
				$oSes->salvar();
				$oSes->commit();
			}
		}
	

	} else {
		if( $_REQUEST['entid'] == 'todos' ){
			
			if( $_REQUEST['escolas'] ){
				
				$arrEntid = explode(",",$_REQUEST['escolas']);

				foreach( $arrEntid as $entid ){
					if(!empty($entid)){		
						$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$entid);
	
						if($sesid){
						
							$sql = "DELETE from par.subescolas_subitenscomposicao WHERE sesid = ".$sesid;
							$db->executar( $sql );
							$db->commit();
							
							$sql = "DELETE from par.subacaoescolas WHERE sesid = ".$sesid;
							$db->executar( $sql );
							$db->commit();
							/*
							$oSes = new SubacaoEscola($sesid);
							$oSes->escid 		 = retornaEscolaPar($entid);
							$oSes->sesano 		 = $_GET['ano'];
							$oSes->sesstatus	 = "A";
							$oSes->sbaid 		 = $_GET['sbaid'];
							$oSes->excluir();
							$oSes->commit();
							*/
						}
					}
				}
			}
		} else {
			
			if($_REQUEST['entid']){	
				$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$_REQUEST['entid']);
				if( $sesid ){
			
					$sql = "DELETE from par.subescolas_subitenscomposicao WHERE sesid = ".$sesid;
					$db->executar( $sql );
					$db->commit();
					
					$sql = "DELETE from par.subacaoescolas WHERE sesid = ".$sesid;
					$db->executar( $sql );
					$db->commit();
					/*
					$oSes = new SubacaoEscola($sesid);
					$oSes->escid 		 = retornaEscolaPar($_REQUEST['entid']);
					$oSes->sesano 		 = $_GET['ano'];
					$oSes->sesstatus	 = "A";
					$oSes->sbaid 		 = $_GET['sbaid'];
					$oSes->excluir();
					$oSes->commit();
					*/
				}
			}
		}
	}

	echo $oSes->montaLista($_GET['sbaid'],$_GET['ano'],$_GET['frmid'],$_GET['ptsid']);
	die;
}

if(!$sbaid){
	echo "<script>alert('Subação não encontrada!');window.close();</script>";
	exit();
}

if( $_SESSION['par']['itrid'] == 2 ){
	if( !$_SESSION['par']['muncod'] ){
		
		$sql = "SELECT iu.itrid, iu.estuf, iu.muncod FROM par.subacao s
				INNER JOIN par.acao a ON a.aciid = s.aciid
				INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
				INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
				WHERE
					sbaid =$sbaid";
		
		$dados = $db->pegaLinha($sql);
		
		extract($dados);
		
		$_SESSION['par']['itrid'] = $itrid;
		
		if( $_SESSION['par']['itrid'] == 2 ){
			$_SESSION['par']['muncod'] = $muncod;
		}else{
			$_SESSION['par']['estuf'] = $estuf;
		}
// 		echo "<script>alert('Erro na sessão!');window.close();</script>";
// 		exit();
	}
}

if( !$_SESSION['par']['itrid'] ){
	
	$sql = "SELECT iu.itrid, iu.estuf, iu.muncod FROM par.subacao s
			INNER JOIN par.acao a ON a.aciid = s.aciid
			INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			WHERE
			sbaid =$sbaid";
	
	$dados = $db->pegaLinha($sql);
	
	extract($dados);
	
	$_SESSION['par']['itrid'] = $itrid;
	
	if( $_SESSION['par']['itrid'] == 2 ){
		$_SESSION['par']['muncod'] = $muncod;
	}else{
		$_SESSION['par']['estuf'] = $estuf;
	}
// 	echo "<script>alert('Falta o Itrid!');window.close();</script>";
// 	exit();
}
?>
<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>PAR 2010 - Plano de Metas - Subação</title>
	
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	</head>
	<body>
		<script type="text/javascript"><!--
			function carregaMuncod( muncod ){
				ano = document.getElementById('ano').value;
				sbaid = document.getElementById('sbaid').value;
				frmid = document.getElementById('frmid').value;
				ptsid = document.getElementById('ptsid').value;
				window.location.href = 'par.php?modulo=principal/subacaoEscolas&acao=A&sbaid='+sbaid+'&ano='+ano+'&muncod='+muncod+'&frmid='+frmid+'&ptsid='+ptsid;
			}
		
			function salvarEscola(entid, obj)
			{
				if( entid ){
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'inserirEscola=1&entid='+entid+'&checado='+obj.checked,
						  success	: function(res) {
								window.opener.jQuery( '#escolas_' + <?= $_GET['ano']; ?> ).html( res );
						  }
					});
				}
			}
			function salvarEscolas()
			{
				var erro = 0;
				$("[class~=obrigatorio]").each(function() { 
					if(!this.value || this.value == "Selecione..."){
						erro = 1;
						alert('Favor preencher todos os campos obrigatórios!');
						this.focus();
						return false;
					}
				});
				if(erro == 0){
					$("#form_subacao").submit();
				}
			}
			function marcarTodos(obj)
			{
				escolas = $("[name='esco']").val();
				if( $("[name=" + obj.name + "]").attr("checked") == true ){
					$("[type=checkbox][name!=" + obj.name + "]").each(function() { 
						$(this).attr("checked",true);
					});
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'inserirEscola=1&entid=todos&escolas='+escolas+'&checado=true',
						  success	: function(res) {
								window.opener.jQuery( '#escolas_' + <?= $_GET['ano']; ?> ).html( res );
						  }
					});
				}else{
					$("[type=checkbox][name!=" + obj.name + "]").each(function() { 
						$(this).attr("checked",false);	
					});
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'inserirEscola=1&entid=todos&escolas='+escolas+'&checado=false',
						  success	: function(res) {
								window.opener.jQuery( '#escolas_' + <?= $_GET['ano']; ?> ).html( res );
						  }
					});
				}
			}
			function qtdePadrao(valor)
			{
				$("[type=text]").each(function() { 
					$(this).val(valor);	
				});
			}
		--></script>
		<form name="form_subacao" id="form_subacao" method="post" action="" >
			<input type="hidden" name="ano" id="ano" value="<?php echo $_GET['ano'] ?>" />
			<input type="hidden" name="sbaid" id="sbaid" value="<?php echo $sbaid ?>" />
			<input type="hidden" name="ptsid" id="ptsid" value="<?php echo $_GET['ptsid'] ?>" />
			<input type="hidden" name="frmid" id="frmid" value="<?php echo $_GET['frmid'] ?>" />
			<input type="hidden" name="requisicao" value="salvarEscolas" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<?php if($_SESSION['par']['itrid'] == 1){ ?>
				<tr style="background-color: #cccccc;">
					<td align="center" colspan="2"><strong>Selecione o Município</strong></td>
				</tr>
			<?php
				$sql = "SELECT
							muncod as codigo,
							mundescricao as descricao
						FROM
							territorios.municipio
						WHERE
							estuf = '{$_SESSION['par']['estuf']}'
						ORDER BY
							mundescricao";
			?>	
				<tr>
					<td colspan="2"><? $db->monta_combo("municipio", $sql,'','Selecione','carregaMuncod','','','','','muncod','',$_REQUEST['muncod']); ?></td>
				</tr>
			<?php } ?>
				
					<?php if($_SESSION['par']['itrid'] == 1): //Estado
								if( $_REQUEST['muncod'] ){					
					
					?>
						<?php $arrTamTd = array(10,25,15,25,15, 10); ?>
						<?php $cabecalho = array("Ação","Município","Código INEP", "Escola","Quantidade Alunos", "QTD. de Salas"); ?>
						<?php /*$sql = "	select DISTINCT
											CASE WHEN ses.escid > 0
												THEN '<input type=\"checkbox\" checked=\"checked\" name=\"escola[' || t.entid || ']\" '
												ELSE '<input type=\"checkbox\" name=\"escola[' || t.entid || ']\" '
											END as acao,
											m.mundescricao,
											t.entcodent,
											t.entnome,
											c.cenquanteducando,
											ted.num_salas_existentes
											--ses.sesquantidade
										from
											entidade.entidade t
										inner join 
											entidade.funcaoentidade f on f.entid = t.entid
							            left join
							            	entidade.entidadedetalhe ed on t.entid = ed.entid
							            left join
											pse.censopse c on t.entid = c.entid
							            inner join
							            	entidade.endereco d on t.entid = d.entid
							            left join 
							            	territorios.municipio m on m.muncod = d.muncod
							            left join 
							            	par.escolas e on e.entid = t.entid
							            left join 
							            	par.subacaoescolas ses on ses.escid = e.escid
							            and 
							            	ses.sesstatus = 'A'
							            and
								        	ses.sesano = '{$_GET['ano']}'
								        and
								        	ses.sbaid = '{$_GET['sbaid']}'
								        left join .tab_dado_escola ted on ted.fk_cod_entidade::character varying = t.entcodent
										where
											(t.entescolanova = false or t.entescolanova is null)
										and
											char_length(t.entcodent)>=8
										and 
											t.entstatus = 'A'
								        and 
								        	f.funid = 3
								        and
								            t.tpcid = 1
								        and
								        	m.estuf = '{$_SESSION['par']['estuf']}'
								        order by
								        	m.mundescricao";*/

								$sql = "select
											CASE WHEN ses.escid > 0
												THEN '<input type=\"checkbox\" onclick=\"salvarEscola(' || e.entid || ', this)\" checked=\"checked\" name=\"escola[' || e.entid || ']\" '
												ELSE '<input type=\"checkbox\" onclick=\"salvarEscola(' || e.entid || ', this)\" name=\"escola[' || e.entid || ']\" '
											END as acao,
											m.mundescricao,
											--mat.fk_cod_entidade as entcodent,
											e.entcodent,
											e.entnome,
											e.entid,
											count( distinct fk_cod_aluno ) as qtde_alunos,
											ted.num_salas_existentes as qtd_salas
										from
											entidade.entidade e
										INNER JOIN 
											entidade.funcaoentidade f ON f.entid = e.entid AND f.funid = 3
							            INNER JOIN 
											entidade.endereco ed ON ed.entid = e.entid
										INNER JOIN 
											territorios.municipio m ON m.muncod = ed.muncod
										left join 
											par.escolas esc on esc.entid = e.entid
										left join 
											par.subacaoescolas ses on ses.escid = esc.escid  and ses.sesstatus = 'A' and ses.sesano = '{$_GET['ano']}' and ses.sbaid = '{$_GET['sbaid']}'
										left join 
											".SCHEMAEDUCACENSO.".tab_entidade ent on e.entcodent = ent.pk_cod_entidade::character varying and ent.fk_cod_municipio = {$_REQUEST['muncod']}
										left join
											".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade= ent.pk_cod_entidade
										left join
											".SCHEMAEDUCACENSO.".tab_matricula mat on mat.fk_cod_entidade = ent.pk_cod_entidade
										where
											--mat.id_status = 1
											--AND 
											(e.entescolanova = false or e.entescolanova is null)
											AND e.entstatus = 'A'
											AND e.tpcid = 1
											AND m.estuf = '{$_SESSION['par']['estuf']}'
											AND m.muncod = '{$_REQUEST['muncod']}'
											AND e.entcodent is not null
										group by
											m.mundescricao, e.entcodent, ses.escid, e.entid, e.entnome, ted.num_salas_existentes 
											--mat.fk_cod_entidade
										order by
											m.mundescricao, e.entnome";
								if(empty( $_REQUEST['muncod']) || empty($_GET['ano']) || empty($_GET['sbaid'])){
									$arrEscolas = array();
								}else{
									$arrEscolas = $db->carregar($sql);
								}

							}
						?>
					<?php elseif($_SESSION['par']['itrid'] == 2): ?> 
						<?php $arrTamTd = array(5,10,35,10,20,10,5,5); ?>
						<?php $tcpid = $_SESSION['par']['muncod'] == '5300108' ? 1 : 3 ?>
						<?php $cabecalho = array("Ação","Código INEP","Escola","Localização","Etapa de Ensino","QTD. de Salas","QTD. de Alunos"); ?>
						<?php 
						
							$sql = "select
										CASE WHEN ses.escid > 0
											THEN '<input type=\"checkbox\" onclick=\"salvarEscola(' || e.entid || ', this)\" checked=\"checked\" name=\"escola[' || e.entid || ']\" '
											ELSE '<input type=\"checkbox\" onclick=\"salvarEscola(' || e.entid || ', this)\" name=\"escola[' || e.entid || ']\" '
										END as acao,
										e.entcodent,
										e.entnome,
										e.entid,
										CASE WHEN ID_LOCALIZACAO = 1 THEN 'urbana' ELSE 'rural' END as localizacao,
										en.no_etapa_ensino as etapa,
										ted.num_salas_existentes as qtd_salas,
										coalesce(ted.num_alunos_existentes,0) as qtd_alunos
									from
										entidade.entidade e
									INNER JOIN entidade.funcaoentidade 			f   ON f.entid 		= e.entid AND f.funid = 3
									INNER JOIN entidade.endereco 	    		ed  ON ed.entid 	= e.entid
									INNER JOIN territorios.municipio    		mu  ON mu.muncod 	= ed.muncod
									LEFT  JOIN par.escolas 						esc ON esc.entid 	= e.entid
									LEFT  JOIN par.subacaoescolas 				ses ON ses.escid 	= esc.escid  AND ses.sesstatus = 'A' AND ses.sesano = '{$_GET['ano']}' AND ses.sbaid = '{$_GET['sbaid']}'
									LEFT  JOIN ".SCHEMAEDUCACENSO.".tab_entidade 	ent 
										INNER JOIN ".SCHEMAEDUCACENSO.".tab_turma 		tu ON tu.fk_cod_entidade = ent.pk_cod_entidade
										INNER JOIN ".SCHEMAEDUCACENSO.".tab_etapa_ensino en ON en.pk_cod_etapa_ensino = tu.fk_cod_etapa_ensino
										ON e.entcodent = ent.pk_cod_entidade::character varying  AND ent.fk_cod_municipio = {$_SESSION['par']['muncod']}
									LEFT  JOIN ".SCHEMAEDUCACENSO.".tab_dado_escola 	ted ON ted.fk_cod_entidade= ent.pk_cod_entidade
									WHERE
										(e.entescolanova = false or e.entescolanova is null)
										AND e.entstatus = 'A'
										AND e.tpcid = {$tcpid}
										AND mu.muncod = '{$_SESSION['par']['muncod']}'
										AND e.entcodent is not null
									ORDER BY
										e.entnome";
// 						ver($tcpid,d);
							$arrEscolas = $db->carregar($sql);
						?>
					<?php endif; ?>
					<tr style="background-color: #cccccc;">
						<td align="center" colspan="2"><strong>Escolas</strong></td>
					</tr>
					<tr>
					<td colspan="2">
					<?php if($arrEscolas): ?>
						<table class="listagem" width="100%" >
							<thead>
								<tr class="SubTituloTabela">
									<?php foreach($cabecalho as $chave => $cab): ?>
										<td width="<?php echo $arrTamTd[$chave] ?>%" class="bold" ><?php echo $cab ?></td>
									<?php endforeach; ?>
								</tr>
							</thead>
							<tr bgcolor="#F9F9F9">
								<?php foreach($cabecalho as $chave => $cab): ?>
									<td width="<?php echo $arrTamTd[$chave] ?>%" align="center" >
										<?php if($chave == 0): ?>
											<input type="checkbox" id="ckb_marca_todos" name="ckb_marca_todos" onclick="marcarTodos(this)" /><br /><label for="ckb_marca_todos" >Marcar Todas</label>
										<?php elseif($chave == count($cabecalho)-1 ): ?>
											<?php //echo campo_texto("qtde_padrao","N","S","",10,12,"[.###]","","right","","","","","","qtdePadrao(this.value)") ?>-
											<br />
										<?php else: ?>
											- 
										<?php endif; ?>
									</td>
								<?php endforeach; ?>
							</tr>
							</table>
							<div id="tbl_escolas" >
								<table class="listagem" width="100%" >
									<?php if( count($arrEscolas) > 17): ?>
										<?php $arrTamTd[count($arrTamTd)-1] = $arrTamTd[count($arrTamTd)-1] - 3 ?>
									<?php endif; ?>
									<?php $num = 0; ?>
									<?php $entids = array(); ?>
									<?php foreach($arrEscolas as $escola): ?>
										<?php $entids[] = $escola['entid']; ?>
										<?php $cor = $num%2 == 1 ? "#f7f7f7" : "#FFFFFF"?>
										<tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
										<?php $escNum = 0; ?>
										<?php foreach($escola as $chave => $esc): ?>
											<?php if($chave != 'entid'){ ?>
											<td width="<?php echo $arrTamTd[$escNum] ?>%" align="<?php echo $chave != "entnome" ? "center" : "left" ?>" ><?php echo  $chave == "sesquantidade" ? campo_texto("sesquantidade[]","N","S","",10,12,"[.###]","","right","","","","", ($esc ? number_format($esc,0,2,'.') : "") ) : $esc ?></td>
											<?php $escNum++; ?>
											<?php } ?>
										<?php endforeach; ?>
										</tr>
										<?php $num++; ?>
									<?php endforeach; ?>
								</table>
							</div>
					<?php endif; ?>
					<?php if( is_array($entids) && $entids[0]){ ?>
						<input type="hidden" id="esco" name="esco" value="<?= implode(",", $entids) ?>"></td>
					<?php } ?>
				</tr>
				<tr>
					<td class="SubTituloDireita"></td>
					<td class="SubTituloEsquerda" >
						<!--<input type="button" value="Salvar" name="btn_salvar" onclick="salvarEscolas()" />-->
						<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
<?php } else { //SE VIER DO MENU

if($_REQUEST['adicionaEscolas']){
	
	global $db;
	
	$ano = $_REQUEST['ano'];
	
	$sql = "SELECT
				'<center><img class=\"middle link\" title=\"Excluir Escola\" src=\"../imagens/excluir.gif\" onclick=\"excluirEscola(this,\'' || t.entid || '\')\"  /></center>' as acao,
				m.mundescricao,
				t.entnome,
				t.entcodent,
				'<input type=\"hidden\" name=\"escola[{$ano}][' || t.entid || ']\" id=\"escola[{$ano}][' || t.entid || ']\" value=\"' || t.entid || '\">' as codigo
			from
				entidade.entidade t
			inner join 
				entidade.funcaoentidade f on f.entid = t.entid
            left join
            	entidade.entidadedetalhe ed on t.entid = ed.entid
            inner join
            	entidade.endereco d on t.entid = d.entid
            left join 
            	territorios.municipio m on m.muncod = d.muncod
			where
				(t.entescolanova = false or t.entescolanova is null)
			AND 
				t.entstatus = 'A'
	        and 
	        	f.funid = 3
	        and
	            t.tpcid = 1
	        and
	        	t.entid IN ({$_REQUEST['escolasselecionadas']})
	        order by
	        	t.entnome,
	        	m.mundescricao";
	      
		$cabecalho = array("Ação","Município","Escola","Código INEP");
		return $db->monta_lista_simples($sql,$cabecalho,100,5,"N","100%");
}
	
	
	
?>
<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>PAR 2010 - Plano de Metas - Subação</title>
	
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	</head>
	<body>
		<script type="text/javascript">
			function salvarEscolasMenu()
			{
				var erro = 0;
				ano = jQuery( '#ano' ).val();
				$("[class~=obrigatorio]").each(function() { 
					if(!this.value || this.value == "Selecione..."){
						erro = 1;
						alert('Favor preencher todos os campos obrigatórios!');
						this.focus();
						return false;
					}
				});
				
				jQuery( '#escolas' ).val( '0' );
				$("[name^='escola[']").each(function(){
					if( $(this).attr("checked") ){
						escolas = jQuery( '#escolas' ).val();
						jQuery( '#escolas' ).val( escolas + ',' + $(this).val() );
					}
				});
				
				escolasselecionadas = jQuery( '#escolas' ).val();
				
				if(erro == 0){
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'adicionaEscolas=1&escolasselecionadas=' + escolasselecionadas,
						  success	: function(res) {
					  			window.opener.jQuery( '#escolas_' + ano ).html( res );
						  }
					});
				}
			}
			function marcarTodos(obj)
			{
				if( $("[name=" + obj.name + "]").attr("checked") == true ){
					$("[type=checkbox][name!=" + obj.name + "]").each(function() { 
						$(this).attr("checked",true);	
					});
				}else{
					$("[type=checkbox][name!=" + obj.name + "]").each(function() { 
						$(this).attr("checked",false);	
					});
				}
			}
			function qtdePadrao(valor)
			{
				$("[type=text]").each(function() { 
					$(this).val(valor);	
				});
			}
		</script>
		<form name="form_subacao" id="form_subacao" method="post" action="" >
			<input type="hidden" name="ppsid" value="<?php echo $ppsid ?>" />
			<input type="hidden" name="requisicao" value="salvarEscolas" />
			<input type="hidden" name="ano" id="ano" value="<?php echo $_REQUEST['ano'] ?>" />
			<input type="hidden" name="escolas" id="escolas" value="" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr style="background-color: #cccccc;">
					<td align="center" colspan="2"><strong>Escolas</strong></td>
				</tr>
				<?php
					$sqlEstado = "SELECT
										m.muncod as codigo,
										m.estuf || ' - ' || m.mundescricao as descricao
									FROM
										territorios.municipio m
									ORDER BY
										descricao";
					
					$municipios = $db->carregar( $sqlEstado );
				
					$Pstmunicipio	= $_REQUEST["municipios"];
				/*	if(isset($Pstmunicipios)){
						$sqlListaMunicipios="select muncod as codigo, estuf as estados, mundescricao as nome from territorios.municipio
											where estuf = '".$Pstmunicipios."' order by mundescricao";
						$municipios = $db->carregar( $sqlListaMunicipios );
					}*/
				?>
				<tr>
					<td>municipios:</td>
						<td>
				      	<select name="municipios" id="municipios">
				      	<?
				      		foreach($municipios as $municipio){
								$chk = "";				  
								$ID	  = $municipio['codigo'];
								$Nome = $municipio['descricao'];
								if( $municipio['codigo'] == $Pstmunicipio ){
									$chk = 'selected="selected"';
								}
				      	?>
				        <option  value="<?=$ID?>" <?=$chk?> ><?=$Nome?></option>
				      	<?
							}
				      	?>
				      	</select>
						<input id="button" type="submit" name="button" value="Filtrar" />
					</td>
				</tr>
				<tr>
					<td colspan="2">
					<?php if($Pstmunicipio){ ?>
						<?php $arrTamTd = array(10,25,15,35,15) ?>
						<?php $cabecalho = array("Ação","Município","Código INEP", "Escola","Quantidade Alunos"); ?>
						<?php $sql = "	select
										--	CASE WHEN ses.escid > 0
										--		THEN 
										--	'<input type=\"checkbox\" checked=\"checked\" name=\"escola[' || t.entid || ']\" '
										--		ELSE 
											'<input type=\"checkbox\" name=\"escola[' || t.entid || ']\" value=\"' || t.entid || '\" >'
										--	END 
											as acao,
											m.mundescricao,
											t.entcodent,
											t.entnome,
											c.cenquanteducando
											--ses.sesquantidade
										from
											entidade.entidade t
										inner join 
											entidade.funcaoentidade f on f.entid = t.entid
							            left join
							            	entidade.entidadedetalhe ed on t.entid = ed.entid
							            left join
											pse.censopse c on t.entid = c.entid
							            inner join
							            	entidade.endereco d on t.entid = d.entid
							            left join 
							            	territorios.municipio m on m.muncod = d.muncod
							            left join 
							            	par.escolas e on e.entid = t.entid
							         --   left join 
							         --   	par.subacaoescolas ses on ses.escid = e.escid
							         --   and 
							         --   	ses.sesstatus = 'A'
							        --    and
								    --    	ses.sesano = '{$_GET['ano']}'
								    --    and
								    --    	ses.sbaid = '{$_GET['sbaid']}'
										where
											(t.entescolanova = false or t.entescolanova is null)
										and
											char_length(t.entcodent)>=8
										and 
											t.entstatus = 'A'
								        and 
								        	f.funid = 3
								        and
								            t.tpcid = 1
								        and
								        	m.muncod = '{$Pstmunicipio}'
								        order by
								        	m.mundescricao";
				      	?>
					<?php } /*elseif($_REQUEST['itrid'] == 6) { ?> 
						<?php $arrTamTd = array(10,20,50,20) ?>
						<?php $tcpid = $_SESSION['par']['muncod'] == '5300108' ? 1 : 3 ?>
						<?php $cabecalho = array("Ação","Código INEP","Escola","Quantidade"); ?>
						<?php $sql = "	select
											CASE WHEN ses.escid > 0
												THEN '<input type=\"checkbox\" checked=\"checked\" name=\"escola[' || t.entid || ']\" '
												ELSE '<input type=\"checkbox\" name=\"escola[' || t.entid || ']\" '
											END as acao,
											t.entcodent,
											t.entnome,
											c.cenquanteducando
											--	ses.sesquantidade
										from
											entidade.entidade t
										inner join 
											entidade.funcaoentidade f on f.entid = t.entid
							            left join
											pse.censopse c on t.entid = c.entid
							            left join
							            	entidade.entidadedetalhe ed on t.entid = ed.entid
							            	and (
								                     entdreg_infantil_creche = '1' or
								                     entdreg_infantil_preescola = '1' or
								                     entdreg_fund_8_anos = '1' or
								                     entdreg_fund_9_anos = '1'
												)
							            inner join 
							            	entidade.endereco d on t.entid = d.entid
							            left join 
							            	par.escolas e on e.entid = t.entid
							            left join 
							            	par.subacaoescolas ses on ses.escid = e.escid 
							            and 
							            	ses.sesstatus = 'A'
							            and
								        	ses.sesano = '{$_GET['ano']}'
								        and
								        	ses.sbaid = '{$_GET['sbaid']}'
										where
											(t.entescolanova = false or t.entescolanova is null)
										AND 
											t.entstatus = 'A'
								        and
								            t.tpcid = {$tcpid}
								        and
								        	d.muncod = '{$_SESSION['par']['muncod']}'
								        order by
								        	t.entnome"; ?>
					<?php } */?>
					<?php if( $Pstmunicipio ){ ?>
					<?php $arrEscolas = $db->carregar($sql) ?>
					<?php if($arrEscolas): ?>
						<table class="listagem" width="100%" >
							<thead>
								<tr class="SubTituloTabela">
									<?php foreach($cabecalho as $chave => $cab): ?>
										<td width="<?php echo $arrTamTd[$chave] ?>%" class="bold" ><?php echo $cab ?></td>
									<?php endforeach; ?>
								</tr>
							</thead>
							<tr bgcolor="#F9F9F9">
								<?php foreach($cabecalho as $chave => $cab): ?>
									<td width="<?php echo $arrTamTd[$chave] ?>%" align="center" >
										<?php if($chave == 0): ?>
											<input type="checkbox" id="ckb_marca_todos" name="ckb_marca_todos" onclick="marcarTodos(this)" /><br /><label for="ckb_marca_todos" >Marcar Todas</label>
										<?php elseif($chave == count($cabecalho)-1 ): ?>
											<?php //echo campo_texto("qtde_padrao","N","S","",10,12,"[.###]","","right","","","","","","qtdePadrao(this.value)") ?>-
											<br />
										<?php else: ?>
											- 
										<?php endif; ?>
									</td>
								<?php endforeach; ?>
							</tr>
							</table>
							<div id="tbl_escolas" >
								<table class="listagem" width="100%" >
									<?php if( count($arrEscolas) > 17): ?>
										<?php $arrTamTd[count($arrTamTd)-1] = $arrTamTd[count($arrTamTd)-1] - 3 ?>
									<?php endif; ?>
									<?php $num = 0; ?>
									<?php foreach($arrEscolas as $escola): ?>
										<?php $cor = $num%2 == 1 ? "#f7f7f7" : "#FFFFFF"?>
										<tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
										<?php $escNum = 0; ?>
										<?php foreach($escola as $chave => $esc): ?>
											<td width="<?php echo $arrTamTd[$escNum] ?>%" align="<?php echo $chave != "entnome" ? "center" : "left" ?>" ><?php echo  $chave == "sesquantidade" ? campo_texto("sesquantidade[]","N","S","",10,12,"[.###]","","right","","","","", ($esc ? number_format($esc,0,2,'.') : "") ) : $esc ?></td>
											<?php $escNum++; ?>
										<?php endforeach; ?>
										</tr>
										<?php $num++; ?>
									<?php endforeach; ?>
								</table>
							</div>
					<?php endif; ?>
					<?php } ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita"></td>
					<td class="SubTituloEsquerda" >
						<input type="button" value="Salvar" name="btn_salvar" onclick="salvarEscolasMenu()" />
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
<?php } ?>