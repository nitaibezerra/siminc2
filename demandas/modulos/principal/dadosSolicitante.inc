<?php

if($_GET["dmdid"]) {
	$dmdid = $_GET["dmdid"];
	$_SESSION['dmdid'] = $dmdid;
} else {
	$dmdid = $_SESSION['dmdid'];
}


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

// Pega o "estado do documento", vinculado à demanda
$esdid = recuperaEstadoDocumento();
//verifica se o estado do documento é finalizada ou cancelada (se sim, bloqueia todos os campos).
if( in_array($esdid, array(DEMANDA_ESTADO_CANCELADO,DEMANDA_GENERICO_ESTADO_CANCELADO,DEMANDA_ESTADO_FINALIZADO,DEMANDA_GENERICO_ESTADO_FINALIZADO,DEMANDA_ESTADO_INVALIDADA,DEMANDA_GENERICO_ESTADO_INVALIDADA,DEMANDA_ESTADO_VALIDADA_FORA_PRAZO)) ){
	$habil = 'N';
}


//desabilita campos para o perfil analista fnde
if ( in_array(DEMANDA_PERFIL_ANALISTA_FNDE, arrayPerfil()) ){
	$habil = 'N';
}

//habilita e desabilita campos para o perfil gerente fabrica de software
if ( in_array(DEMANDA_PERFIL_GERENTE_FSW, arrayPerfil()) ){
	$habil = 'N';
	$mostraWorkflow = 'S';
}



include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

$db->cria_aba( $abacod_tela, $url, '');
  
/*
 * Recupera o "docid"
 * Caso não exista "docid" cadastrado na demanda 
 * Insere o documento e vincula na demanda
 */
$docid = criarDocumento( $dmdid );


$sql = "SELECT 
			u.usucpf,
			u.usunome,
			u.usuemail,
			'(' || u.usufoneddd || ') ' || u.usufonenum AS tel,
			ud.usdcelular AS celular,
			CASE WHEN u.carid not in (9) THEN
					c.cardsc
				ELSE
					u.usufuncao
			END
			as funcao,
			--p.ungdsc,
			ua.unasigla || ' - ' || ua.unadescricao AS setor,
			usucpfdemandante,
			usucpfinclusao
		FROM 
			demandas.demanda AS d
			left JOIN seguranca.usuario AS u ON u.usucpf = d.usucpfdemandante
			left join public.cargo as c ON c.carid = u.carid
			--LEFT JOIN public.unidadegestora AS p ON u.ungcod = p.ungcod 
			left JOIN demandas.unidadeatendimento AS ua ON d.unaid = ua.unaid
			left JOIN demandas.usuariodetalhes AS ud ON ud.usucpf = u.usucpf
 		WHERE
 			--u.usucpf = '".$_SESSION['usucpf']."'
 			d.dmdid = '".$dmdid."'
 			";
$dados = $db->carregar($sql);

//echo $sql;
	
$sql = "SELECT 
			d.dmdnomedemandante as nomedemandante,
			ua.unasigla || ' - ' || ua.unadescricao as setord,
			d.dmdemaildemandante as emaild
		FROM 
			demandas.demanda d
			LEFT JOIN demandas.unidadeatendimento AS ua ON d.unaid = ua.unaid
		WHERE
 			d.dmdid = '".$dmdid."'
 			--AND d.usucpfdemandante is null
 			";
$dados2 = $db->carregar($sql);


monta_titulo( 'Dados do Solicitante - Cód. # '.$dmdid, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>

<?=cabecalhoDemanda(); ?>

<script type="text/javascript" src="/includes/prototype.js"></script>
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
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }  

    function editasolic(cpf)
    {
          location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf="+cpf;
    }  


    function editaDetSolic(cpf)
    {
    	var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/demandas/demandas.php' +
		'?modulo=principal/popUsuarioDetalhes' +
		'&acao=A' +
		'&cpf=' + cpf;
	window.open(
		url,
		'usuarioDetalhes',
		'width=550,height=496,scrollbars=yes,scrolling=no,resizebled=no'
	);
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

<table id="tblFormdadosDemanda" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="25%" align='right' class="SubTituloDireita">Nome:</td>
		<td>
		<? $usunome = $dados[0]['usunome'];  ?>
		<?= campo_texto( 'usunome', 'N', 'N', '', 80, 250, '', '','','','','','');?>
		</td>
		<td rowspan="11" style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
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
	<!-- 
	<tr>
		<td align='right' class="SubTituloDireita">Unidade Gestora:</td>
		<td>
		<? $ungdsc = $dados[0]['ungdsc'];  ?>
		<? ($ungdsc == '')? $ungdsc = "NÃO INFORMADA" : $ungdsc = $ungdsc; ?>
		<?= campo_texto( 'ungdsc', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</td>
	</tr>
	 -->
	<tr>
		<td align='right' class="SubTituloDireita">Setor:</td>
		<td>
		<? $setor = $dados[0]['setor'];  ?>
		<? ($setor == '')? $setor = "NÃO INFORMADO" : $setor = $setor; ?>
		<?= campo_texto( 'setor', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Função / Cargo:</td>
		<td>
		<? $funcao = $dados[0]['funcao'];  ?>
		<? ($funcao == '')? $funcao = "NÃO INFORMADO" : $funcao = $funcao; ?>
		<?= campo_texto( 'funcao', 'N', 'N', '', 80, 250, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">DDD + Telefone:</td>
		<td>
		<? $telefone = $dados[0]['tel'];  ?>
		<?= campo_texto( 'telefone', 'N', 'N', '', 80, 250, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">DDD + Celular:</td>
		<td>
		<? $celular = $dados[0]['celular'] ? '(61) '.formata_fone_fax($dados[0]['celular']) : '';  ?>
		<?= campo_texto( 'celular', 'N', 'N', '', 80, 250, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">E-mail:</td>
		<td>
		<? $usuemail = $dados[0]['usuemail'];  ?>
		<? ($usuemail == '')? $usuemail = "NÃO CADASTRADO" : $usuemail = $usuemail; ?>
		<?//= campo_texto( 'usuemail', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		<input type="text" name="usuemail" size="81" maxlength="250" value="<?=$usuemail?>"  class="disabled"  readonly="readonly" style="width:83ex;text-align : ;"  title='Clique para enviar e-mail' onclick="envia_email('<?=$dados[0]['usucpf']?>');" >
		&nbsp; <a href="javascript:envia_email('<?=$dados[0]['usucpf']?>');"><img src="../imagens/email.gif" title="Envia e-mail" border="0" align="absmiddle"></a>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">&nbsp;</td>
		<td>
			<input type="button" class="botao" name="btalterar" value="Editar" title='Clique para editar os dados do solicitante' onclick="editasolic('<?=$dados[0]['usucpf']?>');" <?if($habil == 'N') echo "disabled";?>>
			&nbsp;
			<input type="button" class="botao" name="btalterar2" value="Editar Outros Detalhes" title='Clique para editar outros detalhes do solicitante' onclick="editaDetSolic('<?=$dados[0]['usucpf']?>');" <?if($habil == 'N') echo "disabled";?>>
		</td>
	</tr>

	<?if($dados2[0]['nomedemandante']){ ?>
	<tr>
		<td align='left' bgcolor="#DCDCDC" colspan="2" style="padding-left:136px"><b>Solicitado em nome de:</b>:</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome Demandante:</td>
		<td>
		<? $nomedemandante = $dados2[0]['nomedemandante'];  ?>
		<? ($nomedemandante == '')? $nomedemandante = "NÃO CADASTRADO" : $nomedemandante = $nomedemandante; ?>
		<?= campo_texto( 'nomedemandante', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Setor:</td>
		<td>
		<? $setord = $dados2[0]['setord'];  ?>
		<? ($setord == '')? $setord = "NÃO CADASTRADO" : $setord = $setord; ?>
		<?= campo_texto( 'setord', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">E-mail:</td>
		<td>
		<? $emaild = $dados2[0]['emaild'];  ?>
		<? ($emaild == '')? $emaild = "NÃO CADASTRADO" : $emaild = $emaild; ?>
		<?= campo_texto( 'emaild', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</td>
	</tr>
	<?}?>

	<?if($dados[0]['usucpfdemandante'] != $dados[0]['usucpfinclusao']){ 
	
		$sql = "SELECT 
			u.usucpf,
			u.usunome,
			u.usuemail
		FROM 
			demandas.demanda AS d
			left JOIN seguranca.usuario AS u ON u.usucpf = d.usucpfinclusao
 		WHERE
 			d.dmdid = '".$dmdid."'
 			";
		$dadost = $db->pegaLinha($sql);
			
	?>
	<tr>
		<td align='left' bgcolor="#DCDCDC" colspan="2" style="padding-left:136px"><b>Cadastrado pelo técnico:</b>:</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome:</td>
		<td>
		<? $nometecnico = $dadost['usunome'];  ?>
		<?= campo_texto( 'nometecnico', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">E-mail:</td>
		<td>
		<? $emailtec = $dadost['usuemail'];  ?>
		<?= campo_texto( 'emailtec', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</td>
	</tr>
	<?}?>

	<tr>
		<td colspan="2" bgcolor="#f5f5f5" height="30" ><b>Outras Demandas do Solicitante:</b></td>
	</tr>
</table>

	<?php
		$sql = "SELECT
				'<div style=\'color:#000000\'>' || d.dmdid || '</div>' AS codigo,
				'<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>',
				CASE
				  WHEN d.docid IS NOT NULL THEN esddsc 
				  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
				END AS situacao,				
				to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') AS dmddatainclusao,
				to_char(d.dmddataconclusao, 'DD-MM-YYYY HH24:MI:SS') AS dmddataconclusao,
				CASE
				  WHEN u.usucpf IS NOT NULL THEN usunome
				  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento	</span>'
				END AS responsavel
			FROM 
				demandas.demanda AS d
				LEFT JOIN workflow.documento doc ON doc.docid 	  = d.docid
				LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
				LEFT JOIN seguranca.usuario u ON u.usucpf 		  = d.usucpfexecutor
			WHERE 
				d.dmdstatus = 'A' 
				AND d.usucpfdemandante   = '".$dados[0]['usucpf']."'
				--AND d.usucpfdemandante = '' 
			ORDER BY 
				 d.dmddatainclusao ASC";
				$cabecalho = array( "Cód" , "Título da Demanda", "Situação", "Data de Abertura", "Data de Encerramento","Solucionado por");
				$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	?>
