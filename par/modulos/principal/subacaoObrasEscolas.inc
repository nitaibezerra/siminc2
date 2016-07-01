<?php
$sbaid = $_REQUEST['sbaid'];


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
	
	$sql = "SELECT * FROM entidade.entidade WHERE entid = ".$_REQUEST['entid'];
	$dado = $db->pegaLinha( $sql );	
	echo "(".$dado['entcodent'].") - ".$dado['entnome']."<input type=\"hidden\" id=\"entcodent\" name=\"entcodent\" value=\"".$dado['entcodent']."\" /><input type=\"hidden\" id=\"entcodent_[]\" name=\"entcodent_[]\" value=\"".$dado['entcodent']."\" />
			<br><a href=\"javascript:inserirEscolas('".$_REQUEST['ano']."','".$_GET['sbaid']."')\">Editar / Inserir Escolas</a>";
	
	/*
	$oSes = new SubacaoEscola();
	
	
	if( $_REQUEST['checado'] == 'true' ){
		
		if( $_REQUEST['entid'] == 'todos' ){
				
			$arrEntid = explode(",",$_REQUEST['escolas']);
			
			foreach( $arrEntid as $entid ){
				$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$entid);
				
				$oSes = new SubacaoEscola($sesid);
				$oSes->escid 		 = retornaEscolaPar($entid);
				$oSes->sesano 		 = $_GET['ano'];
				$oSes->sesstatus	 = "A";
				$oSes->sbaid 		 = $_GET['sbaid'];
				$oSes->salvar();
				$oSes->commit();
			}
			
		} else {
			$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$_REQUEST['entid']);
			
			$oSes = new SubacaoEscola($sesid);
			$oSes->escid 		 = retornaEscolaPar($_REQUEST['entid']);
			$oSes->sesano 		 = $_GET['ano'];
			$oSes->sesstatus	 = "A";
			$oSes->sbaid 		 = $_GET['sbaid'];
			$oSes->salvar();
			$oSes->commit();
		}
	

	} else {
		if( $_REQUEST['entid'] == 'todos' ){
			
			$arrEntid = explode(",",$_REQUEST['escolas']);
			
			foreach( $arrEntid as $entid ){
			
				$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$entid);
				
				if($sesid){
				
					$sql = "DELETE from par.subescolas_subitenscomposicao WHERE sesid = ".$sesid;
					$db->executar( $sql );
					$db->commit();
					
					$oSes = new SubacaoEscola($sesid);
					$oSes->escid 		 = retornaEscolaPar($entid);
					$oSes->sesano 		 = $_GET['ano'];
					$oSes->sesstatus	 = "A";
					$oSes->sbaid 		 = $_GET['sbaid'];
					$oSes->excluir();
					$oSes->commit();
				}
			}
		} else {
			
			$sesid = $oSes->recuperaEscolaPorSubacao($_GET['sbaid'],$_GET['ano'],$_REQUEST['entid']);
			if( $sesid ){
		
				$sql = "DELETE from par.subescolas_subitenscomposicao WHERE sesid = ".$sesid;
				$db->executar( $sql );
				$db->commit();
				
				$oSes = new SubacaoEscola($sesid);
				$oSes->escid 		 = retornaEscolaPar($_REQUEST['entid']);
				$oSes->sesano 		 = $_GET['ano'];
				$oSes->sesstatus	 = "A";
				$oSes->sbaid 		 = $_GET['sbaid'];
				$oSes->excluir();
				$oSes->commit();
			}
		}
	}

	echo $oSes->montaLista($_GET['sbaid'],$_GET['ano'],$_GET['frmid']);

*/
	die;
}

if(!$sbaid){
	echo "<script>alert('Subação não encontrada!');window.close();</script>";
	exit();
}

if( $_SESSION['par']['itrid'] == 2 ){
	if( !$_SESSION['par']['muncod'] ){
		echo "<script>alert('Erro na sessão!');window.close();</script>";
		exit();
	}
}

if( !$_SESSION['par']['itrid'] ){
	echo "<script>alert('Falta o Itrid!');window.close();</script>";
	exit();
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
				divCarregando();
				ano = <?= $_GET['ano']; ?>;
				sbaid = <?= $_GET['sbaid']; ?>;
				window.location.href = 'par.php?modulo=principal/subacaoObrasEscolas&acao=A&sbaid='+sbaid+'&ano='+ano+'&muncod='+muncod;
			}
		
			function salvarEscola(entid, obj)
			{
				if( entid ){
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'inserirEscola=1&entid='+entid,
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
			<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
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
						<?php 
								$sql = "select
											'<input type=\"radio\" onclick=\"salvarEscola(' || e.entid || ', this)\" id=\"escolas\" name=\"escola\">' as acao,
											m.mundescricao,
											mat.fk_cod_entidade as entcodent,
											e.entnome,
											--e.entid,
											count( distinct fk_cod_aluno ) as qtde_alunos,
											ted.num_salas_existentes as qtd_salas
										from
											".SCHEMAEDUCACENSO.".tab_entidade ent 
										inner join
											".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade= ent.pk_cod_entidade
										inner join
											".SCHEMAEDUCACENSO.".tab_matricula mat on mat.fk_cod_entidade = ent.pk_cod_entidade
										inner join
											entidade.entidade e on e.entcodent = ent.pk_cod_entidade::character varying
							            inner join 
							            	territorios.municipio m on m.muncod = ent.fk_cod_municipio::character varying
										left join 
											par.escolas esc on esc.entid = e.entid
										left join 
											par.subacaoescolas ses on ses.escid = esc.escid  and ses.sesstatus = 'A' and ses.sesano = '{$_GET['ano']}' and ses.sbaid = '{$_GET['sbaid']}'
										where
											ent.fk_cod_municipio = {$_REQUEST['muncod']}
											and mat.id_status = 1
											AND (e.entescolanova = false or e.entescolanova is null)
											AND e.entstatus = 'A'
											AND e.tpcid = 1
											AND m.estuf = '{$_SESSION['par']['estuf']}'
											AND m.muncod = '{$_REQUEST['muncod']}'
											AND mat.fk_cod_entidade::varchar NOT IN (SELECT 
															COALESCE( entcodent, '' )
														  FROM 
															obras.preobra p2
														  INNER JOIN par.subacaoobra so    ON so.preid = p2.preid
														  WHERE 
															p2.estuf = '{$_SESSION['par']['estuf']}'
															AND p2.ptoid = 17
															AND so.sbaid = {$_REQUEST['sbaid']}
															AND p2.prestatus <> 'I')
										group by
											m.mundescricao, ses.escid, e.entid, e.entnome, mat.fk_cod_entidade, ted.num_salas_existentes
										order by
											m.mundescricao, e.entnome";
									//dbg($sql);
								$arrEscolas = $db->carregar($sql);
								
							}
						?>
					<?php elseif($_SESSION['par']['itrid'] == 2): ?> 
						<?php $arrTamTd = array(10,20,50,10,10); ?>
						<?php $tcpid = $_SESSION['par']['muncod'] == '5300108' ? 1 : 3 ?>
						<?php $cabecalho = array("Ação","Código INEP","Escola","Quantidade", "QTD. de Salas"); ?>
						<?php 
						
								$sql = "select
											'<input type=\"radio\" onclick=\"salvarEscola(' || e.entid || ', this)\" id=\"escolas\" name=\"escola\">' as acao,
											mat.fk_cod_entidade as entcodent,
											e.entnome,
											--e.entid,
											count( distinct fk_cod_aluno ) as qtde_alunos,
											ted.num_salas_existentes as qtd_salas
										from
											".SCHEMAEDUCACENSO.".tab_entidade ent 
										inner join
											".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade= ent.pk_cod_entidade
										inner join
											".SCHEMAEDUCACENSO.".tab_matricula mat on mat.fk_cod_entidade = ent.pk_cod_entidade
										inner join
											entidade.entidade e on e.entcodent = ent.pk_cod_entidade::character varying
										left join 
											par.escolas esc on esc.entid = e.entid
										left join 
											par.subacaoescolas ses on ses.escid = esc.escid  and ses.sesstatus = 'A' and ses.sesano = '{$_GET['ano']}' and ses.sbaid = '{$_GET['sbaid']}'
										where
											mat.id_status = 1
											AND (e.entescolanova = false or e.entescolanova is null)
											AND e.entstatus = 'A'
											AND e.tpcid = {$tcpid}
											AND fk_cod_municipio = {$_SESSION['par']['muncod']}
											AND mat.fk_cod_entidade::varchar NOT IN (SELECT 
															COALESCE( entcodent, '' )
														  FROM 
															obras.preobra p2
														  INNER JOIN par.subacaoobra so    ON so.preid = p2.preid
														  WHERE 
															p2.muncod = '{$_SESSION['par']['muncod']}'
															AND p2.ptoid = 17
															AND so.sbaid = {$_REQUEST['sbaid']}
															AND p2.prestatus <> 'I')
										group by
											ses.escid, e.entid, e.entnome, mat.fk_cod_entidade, ted.num_salas_existentes
										order by
											e.entnome";
					//	dbg($sql);
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
											<td width="<?php echo $arrTamTd[$escNum] ?>%" align="<?php echo $chave != "entnome" ? "center" : "left" ?>" ><?php echo  $chave == "sesquantidade" ? campo_texto("sesquantidade[]","N","S","",10,12,"[.###]","","right","","","","", ($esc ? number_format($esc,0,2,'.') : "") ) : $esc ?></td>
											<?php $escNum++; ?>
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