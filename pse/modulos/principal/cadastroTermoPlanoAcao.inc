<?php

if(!$_SESSION['conid']){
	?>
	<script>
		alert("É necessário preencher a tela 'Cadastrar Termo de Compromisso.'\n ou sua sessão expirou. Por favor, entre novamente no PSE!");
		location.href='pse.php?modulo=principal/cadastroTermoCompromisso&acao=A';
	</script>
	<?
	exit;
}

if($_REQUEST['entid']){
	
	
	$sql = "SELECT cenquanteducando
			from pse.censopse
			WHERE entid = '".$_REQUEST['entid']."'";
	$qtd = $db->pegaUm($sql);
	
	if($qtd){
		echo $qtd;
	}
	else{
		echo "0";
	}
	
	exit();	
	
}

if($_REQUEST['copiar'] == '1'){
	
	
	$sql = "SELECT  entid,
	            	pamquantedutotal, 
	            	pamquanteduseratend, 
	            	pampercentuseratend,
	            	conid
			from pse.planoacaoc1
			WHERE conid = '".$_SESSION['conid']."'";
	$dados = $db->carregar($sql);
	
	if($dados){
		foreach($dados as $d){
			$sql = "INSERT INTO pse.planoacaoc2
				(
	            	entid,
	            	pacquantedutotal, 
	            	pacquanteduseratend, 
	            	pacpercentuseratend,
	            	conid 
		        ) VALUES (
		        	'".$d['entid']."',
		        	'".$d['pamquantedutotal']."',
		        	'".$d['pamquanteduseratend']."',
		        	'".$d['pampercentuseratend']."',
		        	'".$d['conid']."'
		        ) ";		
			$db->executar($sql);					
		}
		$db->commit();
	}
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A';
		   </script>";
	exit();	
	
}

if($_REQUEST['exluirComp'] == '1'){
	
	if($_REQUEST['pamid']){
		$sql = "DELETE FROM	pse.escolaesf WHERE pamid = ".$_REQUEST['pamid'];
		$db->executar($sql);	
		$sql = "DELETE FROM	pse.planoacaoc1 WHERE pamid = ".$_REQUEST['pamid'];
		$db->executar($sql);	
	}
	if($_REQUEST['pacid']){
		$sql = "DELETE FROM	pse.planoacaoc2 WHERE pacid = ".$_REQUEST['pacid'];
		$db->executar($sql);	
	}
	
						
	$db->commit();
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A';
		   </script>";
	exit();	
	
}

function listarComponenteI(){
	global $db;
	
	if($_REQUEST['plano'] != 'ok'){
		if($_SESSION['permissaoedicao'] == true)
			$img = "<a href=\"#\" onclick=\"alterarComp(\'' || pc.pamid || '\',\'1\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || pc.pamid || '\',\'1\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>";
		else
			$img = '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">';
	}
	else{
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
		echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	}
	$sql = "SELECT ";
	if($_REQUEST['plano'] != 'ok'){
		$sql .=	"
			'<center>$img</center>' as acao,
			ent.entnome,
            pamquantedutotal,
            pamquanteduseratend, 
            pc.pamid as equipe,
			pampercentuseratend 
		";
	}
	else{
		$sql .=	"
			pc.pamid as equipe,
			ent.entnome 
		";
	}
	
		$sql .= "
			FROM pse.planoacaoc1 pc
				INNER JOIN entidade.entidade ent on ent.entid=pc.entid
			WHERE pc.conid = {$_SESSION['conid']}
		";
	
	if($_REQUEST['plano'] != 'ok')
		$cabecalho = array("Ação","Escola","Número de Educandos Total", "Número de Educandos a Serem Atendidos pelo PSE","Equipe de Saúde da Família", "% Percentual de Atendimento");
	else{
		$cabecalho = array("Equipe de Saúde da Família", "Escola");
		echo '
			<center>
				<table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
					<tr bgcolor="#ffffff">
						<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>			
						<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">				
							'. NOME_SISTEMA. '<br/>				
							Ministério da Educação - Ministério da Saúde<br />
						</td>
						<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">					
							Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>					
							Hora da Impressão:' . date( 'd/m/Y - H:i:s' ) . '<br />					
						</td>					
					</tr>				
				</table>
			</center>
		';
	}
	//$alinha = array("center","left","center","center","left","center");
	//$tamanho = array("5%","25%","10%","20%","20%","10%");
	//$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', 'center', '', '',$tamanho,$alinha);
	
	
	//$db->monta_lista_simples( $sql, $cabecalho, 5000, 10, 'N', '100%', 'N' );
	
	$dados = $db->carregar($sql);
	if($dados){
		for($i = 0; $i<=count($dados)-1; $i++ ){
			
			$sqlx = "SELECT
								s.cnecodigocnes ||' - '|| s.cnenomefantasia ||' - '|| s.cneseqequipe as descricao 
						   FROM
						   		pse.escolaesf e
						   INNER JOIN
						   		pse.scnes s ON s.cneid = e.cneid
						   WHERE
						   		e.pamid = ".$dados[$i]['equipe']."
						   ORDER BY s.cnenomefantasia";
			$dadosx = $db->carregarColuna($sqlx);
			if($dadosx){
				$dados[$i]['equipe'] = implode( ";<br>", $dadosx );
			}
			else{ 
				$dados[$i]['equipe'] = '';
			}
		}
	}
	$db->monta_lista_array($dados,$cabecalho,5000,10,'N','center','','',false);
}

if($_REQUEST['plano'] == 'ok'){
	listarComponenteI();
	exit;
}

function inserirDados($componente){
	global $db;
	
	if($componente == '1'){
		
		//verifica se ja existe
		$sql = "SELECT count(entid) as total from pse.planoacaoc1
				WHERE conid = '".$_SESSION['conid']."'
				and entid = '".$_POST['entid1']."'";
		$exite = $db->pegaUm($sql);
		if($exite>0){
			print "<script>
					alert('Escola já cadastrada no Componente I!');
					location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A';
				   </script>";
			exit();
		}
		
		//insere registro
		$vlcomp = $_POST['pampercentuseratend'];
		$vlcomp = str_replace('.','',$vlcomp);
		$vlcomp = str_replace(',','.',$vlcomp);	
		
		$sql = "INSERT INTO pse.planoacaoc1
				(
	            	entid,
	            	pamquantedutotal, 
	            	pamquanteduseratend, 
	            	pampercentuseratend,
	            	conid 
		        ) VALUES (
		        	'".$_POST['entid1']."',
		        	'".$_POST['pamquantedutotal']."',
		        	'".$_POST['pamquanteduseratend']."',
		        	'".$vlcomp."',
		        	'".$_SESSION['conid']."'
		        ) returning pamid";	

		$pamid = $db->pegaUm($sql);
	
	}
	elseif($componente == '2'){
		
		
		//verifica se ja existe
		$sql = "SELECT count(entid) as total from pse.planoacaoc2
				WHERE conid = '".$_SESSION['conid']."'
				and entid = '".$_POST['entid2']."'";
		$exite = $db->pegaUm($sql);
		if($exite>0){
			print "<script>
					alert('Escola já cadastrada no Componente II!');
					location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A';
				   </script>";
			exit();
		}
		
		//insere registro
		$vlcomp = $_POST['pacpercentuseratend'];
		$vlcomp = str_replace('.','',$vlcomp);
		$vlcomp = str_replace(',','.',$vlcomp);	
		
		$sql = "INSERT INTO pse.planoacaoc2
				(
	            	entid,
	            	pacquantedutotal, 
	            	pacquanteduseratend, 
	            	pacpercentuseratend,
	            	conid 
		        ) VALUES (
		        	'".$_POST['entid2']."',
		        	'".$_POST['pacquantedutotal']."',
		        	'".$_POST['pacquanteduseratend']."',
		        	'".$vlcomp."',
		        	'".$_SESSION['conid']."'
		        ) ";

		$db->executar($sql);
		
	}
	
	$db->commit();	
	
	return $pamid;
	
}



function alterarDados($componente){
	global $db;
	
	if($componente == '1'){
		
		$vlcomp = $_POST['pampercentuseratend'];
		$vlcomp = str_replace('.','',$vlcomp);
		$vlcomp = str_replace(',','.',$vlcomp);	
		
		$sql = "UPDATE 
					pse.planoacaoc1
				SET 
		            entid = '".$_POST['entid1']."',
					pamquantedutotal = '".$_POST['pamquantedutotal']."',
					pamquanteduseratend = '".$_POST['pamquanteduseratend']."',
					pampercentuseratend = '".$vlcomp."'
				WHERE 
					pamid = ".$_POST['pamid'];
		
	}
	elseif($componente == '2'){
		
		$vlcomp = $_POST['pacpercentuseratend'];
		$vlcomp = str_replace('.','',$vlcomp);
		$vlcomp = str_replace(',','.',$vlcomp);	
		
		$sql = "UPDATE 
					pse.planoacaoc2
				SET 
		            entid = '".$_POST['entid2']."',
					pacquantedutotal = '".$_POST['pacquantedutotal']."',
					pacquanteduseratend = '".$_POST['pacquanteduseratend']."',
					pacpercentuseratend = '".$vlcomp."'
				WHERE 
					pacid = ".$_POST['pacid'];
				
	}
	
	$db->executar($sql);					
	$db->commit();
}





if($_POST['componente']){
	
	if($_POST['componente'] == '1'){

		$_POST['pamquantedutotal'] = $_POST['pamquantedutotal'] ? $_POST['pamquantedutotal'] : '';
		
		if(!$_POST['pamquantedutotal']){
			print "<script>
						alert('O campo Número de Educandos Total deve ser preenchido!');
						history.back();
				   </script>";
			exit();
		}
		
		if(!$_POST['pamid']) {
			$pamid = inserirDados($_POST['componente']);
		}
		else{
			alterarDados($_POST['componente']);
			$pamid = $_POST['pamid'];
		}
		
		/*** Verifica se existe alguma Equipe ***/
		if( $db->pegaUm("SELECT count(1) FROM pse.escolaesf WHERE pamid = ".$pamid) > 0 )
		{
			/*** Exclui todas as equipes associadas ***/
			$db->executar("DELETE FROM pse.escolaesf WHERE pamid = ".$pamid);
		}
		
		/*** Inclui as equipes se tiver sido informado algum ***/
		if( $_REQUEST['equipe'] && $_REQUEST['equipe'] != "" )
		{
			for($i=0; $i<count($_REQUEST['equipe']); $i++)
			{
				if($pamid && $_REQUEST['equipe'][$i]){
					$db->executar("INSERT INTO pse.escolaesf(pamid,cneid) VALUES(".$pamid.", '".$_REQUEST['equipe'][$i]."')");
				}
			}
			$db->commit();
		}
		
		
		
	}
	elseif($_POST['componente'] == '2'){
		
		$_POST['pacquantedutotal'] = $_POST['pacquantedutotal'] ? $_POST['pacquantedutotal'] : '';
		
		if(!$_POST['pacquantedutotal']){
			print "<script>
						alert('O campo Número de Educandos Total deve ser preenchido!');
						history.back();
				   </script>";
			exit();
		}
		

		if(!$_POST['pacid']) {
			inserirDados($_POST['componente']);
		}
		else{
			alterarDados($_POST['componente']);
		}
		
	}
	
	unset($_POST);
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A';
		   </script>";
	exit();
	
}





$sql = "SELECT mundescricao as municipio, estuf as uf
		FROM territorios.municipio
		WHERE muncod = '".$_SESSION['pse']['muncod']."'";
$mun = $db->pegaLinha($sql);

//pega entid da SECRETARIA MUNICIPAL DE EDUCAÇÃO de um determinado municipio
/*
$sql = "select u.entid, e.entnome 
		from seguranca.usuario u
		left join entidade.entidade e on e.entid = u.entid
		where u.usucpf='".$_SESSION['usucpf']."'";
*/
$sql = "select et.entid, et.entnome from entidade.entidade et
		inner join entidade.endereco ed on ed.entid=et.entid
		inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
		where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'
		order by 1 limit 1";
$secretaria = $db->pegaLinha($sql);

$entid = $secretaria['entid'];


//verifica se o termo territorio é maior que zero.
if($entid){
	$sql = "SELECT conesfatuarapse FROM	pse.contratualizacao WHERE entid = $entid";
	$conesfatuarapse = $db->pegaUm($sql);
	
	if(!$conesfatuarapse || $conesfatuarapse == 0){
		?>
		<script>
			alert("O campo 'Número de Equipes de Saúde da Família que atuarão no PSE' da aba Território de Responsabilidade deve ser maior que zero.");
			location.href='pse.php?modulo=principal/cadastroTermoTerritorio&acao=A';
		</script>
		<?
		exit;
	}
}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';


$titulo = "PSE - Plano de Ação";
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.<br>
<table width="100%" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td width="10%" style="text-align: right;" class="SubTituloDireita">UF:</td>
	<td width="90%" class="SubTituloEsquerda">'.$mun['uf'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">MUNICÍPIO:</td>
	<td class="SubTituloEsquerda">'.$mun['municipio'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">SECRETARIA:</td>
	<td class="SubTituloEsquerda">'.$secretaria['entnome'].'</td>
</tr>
</table>';

echo "<br>";

$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A"),
					  1 => array("id" => 2, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
				  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo( $titulo, $dsctitulo );
echo "<br>";

$db->cria_aba( $abacod_tela, $url, '');


if($_REQUEST['pamid']){
	$sql = "SELECT  
					pamid,
					entid as entid1,
	            	pamquantedutotal,
	            	pamquanteduseratend, 
					pampercentuseratend
		     FROM 
		     		pse.planoacaoc1
		     WHERE 
		     		pamid = {$_REQUEST['pamid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
	if($pampercentuseratend) $pampercentuseratend = number_format($pampercentuseratend,2,",",".");
}

if($_REQUEST['pacid']){
	$sql = "SELECT  
					pacid,
					entid as entid2,
	            	pacquantedutotal,
	            	pacquanteduseratend, 
					pacpercentuseratend
		     FROM 
		     		pse.planoacaoc2
		     WHERE 
		     		pacid = {$_REQUEST['pacid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
	if($pacpercentuseratend) $pacpercentuseratend = number_format($pacpercentuseratend,2,",",".");
}

/*
if(!$_REQUEST['pamid'] && !$_REQUEST['pacid']){
	
	$sql = "SELECT  
					conquanteducando
		     FROM 
		     		pse.contratualizacao
		     WHERE 
		     		conid = {$_SESSION['conid']}";
	$conquanteducando = $db->pegaUm($sql);	
	$pamquantedutotal = $conquanteducando;
	$pacquantedutotal = $conquanteducando;
}
*/


?>

<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="componente" value="">
<input type="hidden" name="pamid" value="<?=$pamid?>">
<input type="hidden" name="pacid" value="<?=$pacid?>">
<input type="hidden" name="total" id="total" value="<?=$pacquantedutotal?>">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td class="SubTituloEsquerda" colspan='4'>
		<br>
	     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Plano de ações e metas do PSE</b>
	     <br><br>
	</td>
	<td align="center" style="background: rgb(238, 238, 238)">
		<img border="0" title="Imprimir Plano de Ação." src="../imagens/print.png" style="cursor: hand" onclick="window.open('pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A&plano=ok', 'termo', 'height=600, width=780, scrollbars=1');">
	</td>
</tr>
<tr>
	<td style="background: rgb(238, 238, 238)" colspan='5'>
		<br>
	     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; As metas anuais pactuadas pelos municípios deverão ser iguais ou maior ao Parâmetro Essencial calculado. O GTI deve definir conjuntamente as escolas federais, estaduais e municipais a serem atendidas<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; no âmbito do PSE, considerando os territórios de abrangência das Unidades Básicas de Saúde, o número de Equipes de Saúde da Família implantadas e o parâmetro essencial de cobertura das ações PSE.</b>
	     <br><br>
	</td>
</tr>
<?php if(!$pacid){?>
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='5'><center>
			 <span style="font-size: 12"><b>COMPONENTE I</b></span>
			 </center></td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Escola:</td>
		<td colspan='2'>
			<?
				/*
				$sql_combo = "SELECT
								ent.entid as codigo,
								ent.entnome as descricao
							  FROM entidade.entidade ent
							  INNER JOIN entidade.endereco eed ON ent.entid = eed.entid 
							  INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = 3
							  WHERE	ent.entstatus = 'A'
							  AND ent.entcodent IS NOT NULL
							  AND ent.entnome IS NOT NULL
							  --AND ent.tpcid in (3,1) 
							  and eed.muncod = '".$_SESSION['pse']['muncod']."'";
				*/
				//echo campo_texto( 'conquantesfmun', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
				$sql_combo = "SELECT
								ent.entid as codigo,
								ent.entnome || ' - ' || cenquanteducando || ' educandos' as descricao
							  FROM pse.censopse cen
							  INNER JOIN entidade.entidade ent ON ent.entid = cen.entid
							  where cen.muncod = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				$ent1 = $db->carregar($sql_combo);
				
				//$db->monta_combo("entid1", $sql_combo, 'S', "Selecione a Escola", 'filtraQtd', '', '', '', 'S', 'entid1');
			?>
			<select name="entid1" class="CampoEstilo obrigatorio" style="width: auto" onchange="filtraQtd(this,1)" id="entid1">
				<option value="">Selecione a Escola</option>
				<?php 
					if($ent1){
						foreach($ent1 as $e){
							?>
							<option value="<?=$e['codigo']?>" <?if($e['codigo'] == $entid1) echo "selected";?>><?=$e['descricao']?></option>
							<?
						}
					}
				?>
			</select>
		</td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Número de Educandos Total:</td>
		<td colspan='2'>
			<?
				//echo campo_texto( 'pamquantedutotal', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
			?>
			<input type="text" class="disabled" title="" style="text-align: right;" readonly="readonly" maxlength="6" size="21" name="pamquantedutotal" id="pamquantedutotal" value="<?=$pamquantedutotal?>">
		</td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Número de Educandos a Serem Atendidos pelo PSE:</td>
		<td colspan='2'>
			<?//=campo_texto( 'pamquanteduseratend', 'S', 'S', '', 20, 6, '######', '', 'right', '', 0, '', 'calcv(this,1)', '', '');?>
			<input type="text" class="obrigatorio clsMouseFocus" title="" style="text-align: right;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal('######',this.value);calcv(this,1)" maxlength="6" size="21" name="pamquanteduseratend" id="pamquanteduseratend" value="<?=$pamquanteduseratend?>">
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	<tr>  
		<td width="50%" class="SubTituloDireita" colspan='2'>Equipe de Saúde da Família:</td>
		<td colspan='2'>
			<?	
				$sql_combo = "SELECT
								cneid as codigo,
								cnecodigocnes ||' - '|| cnenomefantasia ||' - '|| cneseqequipe as descricao
							  FROM pse.scnes
							  WHERE	muncod = '".$_SESSION['pse']['muncod']."'
							  order by cnenomefantasia";
				/*
				//echo campo_texto( 'conquantesfmun', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
				$db->monta_combo("cneid", $sql_combo, 'S', "Selecione a Equipe de Saúde da Família", '', '', '', '', 'S', 'cneid');
				*/
				if( $pamid )
				{
					/*** Se existem fiscais cadastrados ***/
					if( $db->pegaUm("SELECT count(1) FROM pse.escolaesf WHERE pamid = ".$pamid) > 0 )
					{
						$sql_carregados = "SELECT
										 		s.cneid as codigo,
												s.cnecodigocnes ||' - '|| s.cnenomefantasia ||' - '|| s.cneseqequipe as descricao 
										   FROM
										   		pse.escolaesf e
										   INNER JOIN
										   		pse.scnes s ON s.cneid = e.cneid
										   WHERE
										   		e.pamid = ".$pamid."
										   ORDER BY s.cnenomefantasia";
						$equipe = $db->carregar( $sql_carregados );
					}
				}
						
				combo_popup( 'equipe', $sql_combo, 'Selecione a(s) Equipe(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, null, null, null, true, true, null, true );
				echo obrigatorio();			
			?>
		</td>
	</tr>
	<tr>  
		<td width="50%" class="SubTituloDireita" colspan='2'>Percentual de Atendimento:</td>
		<td colspan='2'>
			<?//=campo_texto( 'pampercentuseratend', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '', '', '', '');?>
			<input type="text" class="disabled" title="" style="text-align: right;" readonly="readonly" maxlength="6" size="21" name="pampercentuseratend" id="pampercentuseratend" value="<?=$pampercentuseratend?>">%
		</td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="validaFormC1();">
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A';">
					</td>
					<td align="right">
						<?php 
							$sql = "SELECT  count(entid) as total
									from pse.planoacaoc2
									WHERE conid = '".$_SESSION['conid']."'";
							$copiaTotal = $db->pegaUm($sql);
							if($copiaTotal <= 0){
								?>
								<input type="button" class="botao" name="btCopiarC1" id="btCopiarC1" value="Copiar dados para componente II" onclick="if(confirm('Deseja realmente copiar os dados do componente I para o componente II?')){location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A&copiar=1';}">
								<?
							}
							?>	
							&nbsp;				
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td colspan='4'>
			<div id="listaC1" style="height: 100px; overflow-y: auto; overflow-x: hidden;">
				<?php 
					listarComponenteI();														
				?>
			<div>
		</td>
	</tr>
	<tr>
		<td style="background: rgb(198, 198, 198);" colspan='4' nowrap="nowrap">
			<?
				$sql = "SELECT concoberturacomp1 as total1 FROM pse.contratualizacao 
						WHERE conid = {$_SESSION['conid']}";
				$total1 = $db->pegaUm($sql);
				if(!$total1) $total1 = 0;
			
				$sql = "SELECT sum(pamquanteduseratend) as total2 FROM pse.planoacaoc1 
						WHERE conid = {$_SESSION['conid']}";
				$total2 = $db->pegaUm($sql);
				if(!$total2) $total2 = 0;
				
				if($total1 > $total2) $saldo = "<font color=red><b>".($total1-$total2)."</b></font>";
				else $saldo = "<font color=black><b>".($total1-$total2)."</b></font>";
			?>
			<table border=0 width="100%">
				<tr>
					<td>
						<!-- <b>TOTAL:<b/>&nbsp; -->
					</td>
					<td align="right">
						<font size="2px">
						<b>Parâmetro Componente I: <?=$total1?><br>
						   Total de Educandos a Serem Atendidos pelo PSE: <?=$total2?><br>
						   <font size="3px">Saldo: <?=str_replace("-","",$saldo)?></font><b/>
						   <br />
						   <span style="color:red;">SALDO EM VERMELHO INDICA QUE A META PACTUADA AINDA NÃO FOI ATINGIDA !</span>
						</font>
					</td>
				</tr>
			</table>
		</td>
	</tr>	
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			&nbsp;
		</td>
	</tr>	
<?}?>
<?php if(!$pamid){?>
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'><center>
			 <span style="font-size: 12"><b>COMPONENTE II</b></span>
			 </center></td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Escola:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								ent.entid as codigo,
								ent.entnome || ' - ' || cenquanteducando as descricao
							  FROM pse.censopse cen
							  INNER JOIN entidade.entidade ent ON ent.entid = cen.entid
							  where cen.muncod = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				$ent2 = $db->carregar($sql_combo);
				//echo campo_texto( 'conquantesfmun', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
				//$db->monta_combo("entid2", $sql_combo, 'S', "Selecione a Escola", 'filtraQtd', '', '', '', 'S', 'entid2');
			?>
			<select name="entid2" class="CampoEstilo obrigatorio" style="width: auto" onchange="filtraQtd(this,2)" id="entid2">
				<option value="">Selecione a Escola</option>
				<?php 
					if($ent2){
						foreach($ent2 as $e2){
							?>
							<option value="<?=$e2['codigo']?>" <?if($e2['codigo'] == $entid2) echo "selected";?>><?=$e2['descricao']?></option>
							<? 
						}
					}
				?>
			</select>
		</td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Número de Educandos Total:</td>
		<td colspan='2'>
			<?
				//echo campo_texto( 'pacquantedutotal', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
			?>
			<input name="pacquantedutotal" id="pacquantedutotal" class="disabled" type="text" title="" style="text-align: right;" readonly="readonly" maxlength="6" size="21" value="<?=$pacquantedutotal?>">
		</td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Número de Educandos a Serem Atendidos pelo PSE:</td>
		<td colspan='2'>
			<?//=campo_texto( 'pacquanteduseratend', 'S', 'S', '', 20, 6, '######', '', 'right', '', 0, '', 'calcv(this,2)', '', '');?>
			<input type="text" class="obrigatorio clsMouseFocus" title="" style="text-align: right;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal('######',this.value);calcv(this,2)" maxlength="6" size="21" name="pacquanteduseratend" id="pacquanteduseratend" value="<?=$pacquanteduseratend?>">
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		</td>
	</tr>
	<tr>  
		<td width="50%" class="SubTituloDireita" colspan='2'>Percentual de Atendimento:</td>
		<td colspan='2'>
			<?//=campo_texto( 'pacpercentuseratend', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '', '', '', '');?>
			<input type="text" class="disabled" title="" style="text-align: right;" readonly="readonly" maxlength="6" size="21" name="pacpercentuseratend" id="pacpercentuseratend" value="<?=$pacpercentuseratend?>">%
		</td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<input type="button" class="botao" name="btSalvarC2" id="btSalvarC2" value="Salvar" onclick="validaFormC2();">
			&nbsp;
			<input type="button" class="botao" name="btVoltarC2" id="btVoltarC2" value="Voltar" onclick="location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A';">
		</td>
	</tr>
	<tr>
		<td colspan='4'>
			<div id="listaC2" style="height: 100px; overflow-y: auto; overflow-x: hidden;">
				<?php 
					if($_SESSION['permissaoedicao'] == true){
						$img = "<a href=\"#\" onclick=\"alterarComp(\'' || pc.pacid || '\',\'2\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || pc.pacid || '\',\'2\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>";
					} else {
						$img = '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">';
					}
					$sql = "SELECT
								'<center>$img</center>' as acao, 
								ent.entnome,
				            	pacquantedutotal,
				            	pacquanteduseratend, 
								pacpercentuseratend
							FROM pse.planoacaoc2 pc
							INNER JOIN entidade.entidade ent on ent.entid=pc.entid
							WHERE pc.conid = {$_SESSION['conid']}";
					
					$cabecalho 	= array( "Ação", "Escola","Número de Educandos Total", "Número de Educandos a Serem Atendidos pelo PSE", "% Percentual de Atendimento");
					//$alinha = array("center","left","center","center","center");
					//$tamanho = array("5%","45%","10%","20%","10%");
					//$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', 'center', '', '',$tamanho,$alinha);
					//$db->monta_lista_simples( $sql, $cabecalho, 5000, 10, 'N', '100%', 'N' );
					$dados2 = $db->carregar($sql);
					$db->monta_lista_array($dados2,$cabecalho,5000,10,'N','center','','',false);				
				?>
			<div>
		</td>
	</tr>
		<tr>
		<td style="background: rgb(198, 198, 198);" colspan='4' nowrap="nowrap">
			<?
				unset($total1);
				unset($total2);
				unset($saldo);
				$sql = "SELECT concoberturacomp2 as total1 FROM pse.contratualizacao 
						WHERE conid = {$_SESSION['conid']}";
				$total1 = $db->pegaUm($sql);
				if(!$total1) $total1 = 0;
			
				$sql = "SELECT sum(pacquanteduseratend) as total2 FROM pse.planoacaoc2 
						WHERE conid = {$_SESSION['conid']}";
				$total2 = $db->pegaUm($sql);
				if(!$total2) $total2 = 0;
				
				if($total1 > $total2) $saldo = "<font color=red><b>".($total1-$total2)."</b></font>";
				else $saldo = "<font color=black><b>".($total1-$total2)."</b></font>";
			?>
			<table border=0 width="100%">
				<tr>
					<td>
						<!-- <b>TOTAL:<b/>&nbsp; -->
					</td>
					<td align="right">
						<font size="2px">
						<b>Parâmetro Componente II: <?=$total1?><br>
						   Total de Educandos a Serem Atendidos pelo PSE: <?=$total2?><br>
						   <font size="3px">Saldo: <?=str_replace("-","",$saldo)?></font><b/>
						   <br />
						   <span style="color:red;">SALDO EM VERMELHO INDICA QUE A META PACTUADA AINDA NÃO FOI ATINGIDA !</span>
						</font>
					</td>
				</tr>
			</table>
		</td>
	</tr>	
	
	
<?}?>
	
</table>


</form>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">

var d = document.formulario;

function filtraQtd(id,comp){

	var total 			 = document.getElementById('total');

	var pamquantedutotal = document.getElementById('pamquantedutotal');
	var pacquantedutotal = document.getElementById('pacquantedutotal');
	
	var pampercentuseratend = document.getElementById('pampercentuseratend');
	var pacpercentuseratend = document.getElementById('pacpercentuseratend');
	
	if(id.value != ''){
		$.ajax({
	   		type: "POST",
	   		url: "pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A",
	   		data: "entid="+id.value,
	   		success: function(msg){
				if(comp == 1) {
					total.value = msg;
					pamquantedutotal.value = msg;
					calcv(pamquantedutotal, 1);
				}
				else if(comp == 2){
					total.value = msg;
					pacquantedutotal.value = msg;
					calcv(pacquantedutotal, 2);
				}
	   		}
	 		});
	}
	else{
		if(comp == 1) {
			pamquantedutotal.value='0';
			pampercentuseratend.value='0';
		}
		else if(comp == 2){
			pacquantedutotal.value='0';
			pacpercentuseratend.value='0';
		}
	}

}





function validaFormC1(){

	if(d.entid1.value == ''){
		alert('Favor Preencher o campo obrigatório: Escola.');
		d.entid1.focus();
		return false;
	}
	if(d.pamquanteduseratend.value == ''){
		alert('Favor Preencher o campo obrigatório: Número de Educandos a Serem Atendidos pelo PSE.');
		d.pamquanteduseratend.focus();
		return false;
	}
	/*
	if(d.cneid.value == ''){
		alert('Favor Preencher o campo obrigatório: Equipes CNES.');
		d.cneid.focus();
		return false;
	}
	*/
	var equipe = document.getElementById( 'equipe' );
	selectAllOptions( equipe );
	
	if ( !equipe.options[0].value ){
		alert( 'Favor selecionar a Equipe de Saúde da Família!' );
		return false;
	}
		

	d.pacid.value='';
	d.componente.value='1';
	d.submit();

}

function validaFormC2(){

	var entid2 = document.getElementById('entid2');
	var pacquanteduseratend = document.getElementById('pacquanteduseratend'); 
	 
	if(entid2.value == ''){
		alert('Favor Preencher o campo obrigatório: Escola.');
		entid2.focus();
		return false;
	}
	if(pacquanteduseratend.value == ''){
		alert('Favor Preencher o campo obrigatório: Número de Educandos a Serem Atendidos pelo PSE.');
		pacquanteduseratend.focus();
		return false;
	}

	d.pamid.value='';
	d.componente.value='2';
	d.submit();

}


function alterarComp(id, comp){

	if(comp == '1'){
		//d.pamid.value=id;
		location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A&pamid='+id;		
	}
	else if(comp == '2'){
		//d.pacid.value=id;
		location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A&pacid='+id;
	}
	
}


function excluirComp(id, comp){

	if(confirm('Deseja realmente excluir este item?')){
		if(comp == '1'){
			//d.pamid.value=id;
			location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A&exluirComp=1&pamid='+id;		
		}
		else if(comp == '2'){
			//d.pacid.value=id;
			location.href='pse.php?modulo=principal/cadastroTermoPlanoAcao&acao=A&exluirComp=1&pacid='+id;
		}
	}
}






function calcv(obj, comp){

	var pamquantedutotal = document.getElementById('pamquantedutotal');
	var pacquantedutotal = document.getElementById('pacquantedutotal');
	
	var pampercentuseratend = document.getElementById('pampercentuseratend');
	var pacpercentuseratend = document.getElementById('pacpercentuseratend');

	var pamquanteduseratend = document.getElementById('pamquanteduseratend');
	var pacquanteduseratend = document.getElementById('pacquanteduseratend');

	if(comp == 1){
		if(pamquantedutotal.value > 0){

			if(parseFloat(pamquanteduseratend.value) > 0){ 
				if(parseFloat(pamquanteduseratend.value) <= parseFloat(pamquantedutotal.value)){ 
					pampercentuseratend.value = (pamquanteduseratend.value * 100) / pamquantedutotal.value;
				}
				else{
						alert("Número de Educandos a Serem Atendidos pelo PSE não pode ser maior que o Número de Educandos Total.");
						pamquanteduseratend.value = 0;
						pampercentuseratend.value = 0;
					}
			}
		}
		else{
			pampercentuseratend.value = 0;
		}
		pampercentuseratend.value = float2moeda(pampercentuseratend.value);
	}
	else if(comp == 2){
		if(pacquantedutotal.value > 0){
			

			if(parseFloat(pacquanteduseratend.value) > 0){ 
				if(parseFloat(pacquanteduseratend.value) <= parseFloat(pacquantedutotal.value)){ 
					pacpercentuseratend.value = (pacquanteduseratend.value * 100) / pacquantedutotal.value;
				}
				else{
						alert("Número de Educandos a Serem Atendidos pelo PSE não pode ser maior que o Número de Educandos Total.");
						pacquanteduseratend.value = 0;
						pacpercentuseratend.value = 0;
					}
			}
			
		}
		else{
			pacpercentuseratend.value = 0;
		}
		pacpercentuseratend.value = float2moeda(pacpercentuseratend.value);
	}
	
	
}

function float2moeda(num) {
	   x = 0;
	   if(num<0) {
	      num = Math.abs(num);
	      x = 1;
	   }
	   if(isNaN(num)) num = "0";
	      cents = Math.floor((num*100+0.5)%100);
	   num = Math.floor((num*100+0.5)/100).toString();
	   if(cents < 10) cents = "0" + cents;
	      for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++)
	         num = num.substring(0,num.length-(4*i+3))+'.'
	               +num.substring(num.length-(4*i+3));
	    ret = num + ',' + cents;
	    if (x == 1) ret = ' - ' + ret;
	    return ret;
}

<?php if($_SESSION['permissaoedicao'] == false){?>
	var obj = document.getElementsByTagName("input");
	//var objImg = document.getElementsByTagName("img");
	var total = document.getElementsByTagName("input").length;
	//var totalImg = document.getElementsByTagName("img").length;
	
	for(i=0; i<total; i++){
		obj[i].disabled = true;
	}
	
	/*for(i=0; i<totalImg; i++){
		if( objImg[i].src == '../imagens/alterar.gif' ){
		}
		//obj[i].disabled = true;
	}*/
<?php }?>


</script>
