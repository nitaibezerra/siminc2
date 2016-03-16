<?php
$dmdid = $_SESSION['dmdid'];

$habil = 'N';
$mostraWorkflow = 'N';

//verifica se o usuário pode editar a demanda
$verificaEditaDemanda = verificaEditaDemanda();
if($verificaEditaDemanda == true){
	$habil = 'S';
	$mostraWorkflow = 'S';
}
else{
	$habil = 'N';
	$mostraWorkflow = 'N';
}


//verifica se a demanda é do demandante mesmo.
$sql = "select count(dmdid) as total from demandas.demanda where dmdid=".$dmdid." AND usucpfdemandante = '" . $_SESSION['usucpf'] . "'"; 
$total = $db->PegaUm($sql);
if ( $total > 0 ){
	$habil = 'S';
	$mostraWorkflow = 'N';
}

// Pega o "estado do documento", vinculado à demanda
$esdid = recuperaEstadoDocumento();
//verifica se o estado do documento é finalizada ou cancelada (se sim, bloqueia todos os campos).
//if( in_array($esdid, array(DEMANDA_ESTADO_CANCELADO,DEMANDA_ESTADO_FINALIZADO,DEMANDA_ESTADO_INVALIDADA,DEMANDA_GENERICO_ESTADO_FINALIZADO,DEMANDA_GENERICO_ESTADO_INVALIDADA,DEMANDA_GENERICO_ESTADO_CANCELADO)) ){
//	$habil = 'N';
//}


//habilita e desabilita campos para o perfil gerente e analista fabrica de software
if ( ( in_array(DEMANDA_PERFIL_GERENTE_FSW, arrayPerfil()) || in_array(DEMANDA_PERFIL_ANALISTA_FSW, arrayPerfil()) ) && in_array($esdid, array(DEMANDA_GENERICO_ESTADO_EM_ANALISE, DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO, DEMANDA_GENERICO_ESTADO_AGUARDANDO_VALIDACAO)) ){
	$habil = 'S';
	$mostraWorkflow = 'S';
}



include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ ."includes/cabecalho.inc";

/*
 * Recupera o "docid"
 * Caso não exista "docid" cadastrado na demanda 
 * Insere o documento e vincula na demanda
 */
$docid = criarDocumento( $dmdid );

print '<br>';
$db->cria_aba( $abacod_tela, $url, '');

function inserirObs(){
	global $db;
	
	$sql = "INSERT INTO demandas.observacoes (
				usucpf,
				dmdid,
				obsdsc,
				obsstatus,
				obsdata
			) VALUES (
				'".$_SESSION['usucpf']."', 
				'".$_SESSION['dmdid']."',
				'".simec_addslashes($_POST['obsdsc'])."',
				'A',
				now()
			);";
	$db->executar($sql);
	$db->commit();
}


if($_POST['obsdsc'] && $_SESSION['dmdid'] && $_SESSION['usucpf']){
	inserirObs();

	enviaEmailObservacao();
		
	print "<script>
				alert('Operação realizada com sucesso!');
				location.href = window.location; 
		   </script>";	
}

monta_titulo( 'Observação - Cód. # '.$dmdid, '' );
?>
<html>
 <head>
  <script type="text/javascript" src="/includes/prototype.js"></script>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
  <script src="../fabrica/js/autocomplete/jquery-1.7.2.js"></script>
    <script type="text/javascript">
    jQuery.noConflict();
    jQuery(document).ready(function(){
    <?php 
        $estadoAtualDaDemanda =  wf_pegarEstadoAtual($docid);
        if( in_array($estadoAtualDaDemanda['esdid'], array(
        DEMANDA_ESTADO_EM_ANALISE
        ,DEMANDA_ESTADO_EM_ATENDIMENTO
        ,DEMANDA_GENERICO_ESTADO_EM_ANALISE	       				
        ,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO			   				
        ) ) ) { ?>
            jQuery('a[title^="Avaliação da demanda"]').removeAttr("href").css('color', '#B5B5B5').css('text-decoration', 'none');
    <?php } ?>
    });
    </script>
  <script type="text/javascript">
	function validaForm(){
		if(document.formObservacao.obsdsc.value == ''){
			alert ('O campo Observação deve ser preenchido.');
			document.formObservacao.obsdsc.focus();
			return false;
		}
	}	

	function exibeHistoricoDem( docid )
	{
		var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
			'?modulo=principal/tramitacao' +
			'&acao=C' +
			'&docid=' + docid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	}				
	
  </script>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?=cabecalhoDemanda(); ?>
<form id="formObservacao" name="formObservacao" action="" method="post" onsubmit="return validaForm();">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita">Observação:</td>
		<td>
			<?=campo_textarea('obsdsc', 'N ', $habil, '', 80, 5, 4000); ?>
		</td>
		<td rowspan="1" style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
			<?php 
				if($mostraWorkflow == 'S'){
					// Barra de estado atual e ações e Historico
					wf_desenhaBarraNavegacao( $docid , array( 'unicod' => '' ) );	
					//Barra outras opções
					criarNovaDemanda(); //www/demandas/_funcoes.php
				}else{
					?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>estado atual</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<?php 
												if($esdid){
													$sqlesd = "SELECT esddsc
															FROM workflow.estadodocumento
															WHERE esdid = $esdid";
													
													echo $db->pegaUm($sqlesd);
												}
												else{
													echo "Em Processamento";
												}
												?>					
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>ações</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												você não possui permissão
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>histórico</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<img style="cursor: pointer;" src="http://<?=$_SERVER['SERVER_NAME']?>/imagens/fluxodoc.gif"	 onclick="exibeHistoricoDem( '<?=$docid?>' );">
										</td>
									</tr>
									
						</table>					
					<? 
				}
			?>
		</td>	

	</tr>

	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td colspan="2">
	    	<input type='submit' class='botao' value='Salvar' name='cadastrar' <?= $habil == 'N' ? 'disabled="disabled"' : ''?> />&nbsp;
	    	<!-- <input type='button' class='botao' value='Fechar' name='fechar' onclick='window.close();' />	-->    	
		</td>			
	</tr>	
</table>
</form>
<?php
$sql = "SELECT 
		 to_char(o.obsdata, 'DD-MM-YYYY HH24:MI:SS') AS data,
		 o.obsdsc,  
		 CASE WHEN o.obslog = 'A' THEN 'SISTEMA DEMANDAS'
		 ELSE u.usunome
		 END AS nome
		 
		FROM
		 demandas.observacoes AS o 
		 LEFT JOIN seguranca.usuario AS u ON u.usucpf = o.usucpf
	    WHERE
	     o.dmdid = '{$dmdid}' AND 
	     o.obsstatus = 'A' 
	    ORDER BY
	     o.obsid DESC";
$cabecalho = array("Postado","Observação","Autor");			
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>

<form name="formdp">
<?php
$sql = "SELECT
		 d.dmdid || ' ',
		 d.dmdtitulo, 
		 to_char(o.obsdata, 'DD-MM-YYYY HH24:MI:SS') AS data,
		 o.obsdsc,  
		 u.usunome
		FROM
			demandas.observacoes AS o 
		LEFT JOIN seguranca.usuario AS u ON u.usucpf = o.usucpf 
		LEFT JOIN demandas.demanda as d ON d.dmdid = o.dmdid 
	    WHERE
	     o.dmdid in (select de.dmdidorigem from demandas.demanda as de where de.dmdid = ".$dmdid.") AND 
	     o.obsstatus = 'A' 
	    ORDER BY
	     o.obsid DESC";
$aux = $db->PegaUm($sql);
if($aux){
	echo "<br><br><div><table width='95%' align='center'><tr><td><b>Observações da Demanda Principal</b></td></tr></table></div>";
	$cabecalho = array("Cód. Demanda","Assunto","Postado","Observação","Autor");			
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
}
?>
</form>

<form name="formsd">
<?php
$sql = "SELECT
		 d.dmdid || ' ',
		 d.dmdtitulo, 
		 to_char(o.obsdata, 'DD-MM-YYYY HH24:MI:SS') AS data,
		 o.obsdsc,  
		 u.usunome
		FROM
			demandas.observacoes AS o 
		LEFT JOIN seguranca.usuario AS u ON u.usucpf = o.usucpf 
		LEFT JOIN demandas.demanda as d ON d.dmdid = o.dmdid 
	    WHERE
	     o.dmdid in (select de.dmdid from demandas.demanda as de where de.dmdidorigem = ".$dmdid.") AND 
	     o.obsstatus = 'A' 
	    ORDER BY
	     o.obsid DESC";
$aux = $db->PegaUm($sql);
if($aux){
	echo "<br><br><div><table width='95%' align='center'><tr><td><b>Observações das SubDemandas</b></td></tr></table></div>";
	$cabecalho = array("Cód. Demanda","Assunto","Postado","Observação","Autor");			
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
}
?>
</form>

</body>
</html>

<?
//rotina para atualizar tecnico classificador
/*
if($_SESSION['usucpf'] = ''){
	
	ini_set("memory_limit", "5012M");
	
	$sql = "select dmdid, usucpf, obsdata from demandas.observacoes
			where obsdsc ilike '%alterou os seguintes campos da Classificação%'
			order by dmdid desc, obsdata desc";
	
	$dados = $db->carregar($sql);
	
	if($dados){
		
		foreach ($dados as $d){
			
			$sql = "UPDATE demandas.demanda SET
						usucpfclassificador = '".$d['usucpf']."', 
						dmddataclassificacao = '".$d['obsdata']."'
					WHERE dmdid = '".$d['dmdid']."'";
			echo "<br>".$d['dmdid'];
			//flush();
			$db->executar($sql);
			$db->commit();
		}
		
		echo "<br>ACABOUUUUUUU";
	}
	
}
*/
?>