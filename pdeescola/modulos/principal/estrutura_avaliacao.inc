<?php
$da = explode('-', date('d-m-Y'));
$de = explode('-', DATA_LIMITE_EQUIPE);
$dc = explode('-', DATA_LIMITE_COMITE);
$perfis = arrayPerfil();

$dataAtual = mktime($day = $da[0], $month = $da[1], $year = $da[2]);
$dataEquipe = mktime ($day = $de[0], $month = $de[1], $year = $de[2]); 
$dataComite = mktime ($day = $dc[0], $month = $dc[1], $year = $dc[2]);

$limEquipe = ( $dataEquipe < $dataAtual);
$limComite = ( $dataComite < $dataAtual);

$pTodos = (in_array( PDEESC_PERFIL_CONSULTA, $perfis) || in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfis) || in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis));
$pComite = (in_array( PDEESC_PERFIL_COMITE_ESTADUAL, $perfis ) || in_array( PDEESC_PERFIL_COMITE_MUNICIPAL, $perfis ));
$pEquipe = ((in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) || (in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis)));


header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

if (!$_SESSION['entid']){
	die('<script>
			history.go(-1);
		 </script>');
}

// Tratamento Exercicio base de comparacao - exibir dados do ano de exercicio ate ano vigente.
if($_SESSION["exercicio"] != $_SESSION['exercicio_atual']) {
  echo "<script language=\javascript\" type=\"text/javascript\">
				alert('Nenhum dado para este ano Exercício Base de Comparação.');
			window.history.go(-1);
		</script>";
	exit;	
			}

		
$entid = $_SESSION['entid'];

//checagem de documentos
include_once APPRAIZ . 'includes/workflow.php';
criarDocumento( $entid );
$docid 			   = pegarDocid($entid);
$_SESSION['pdeid'] = pegarPdeid($entid);

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
echo montarAbasArray(carregaAbas($docid), "/pdeescola/pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A");

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Formulário de Diagnóstico';
monta_titulo($titulo_modulo,'Respondido<img src="/imagens/check_p.gif" style="border:0;">
							 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						     Não Respondido<img src="/imagens/atencao.png" style="border:0;">');
?>
<?php $estAtual = wf_pegarEstadoAtual($docid);?>
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<script>function verificaPendencia(){return windowOpen('pdeescola.php?modulo=principal/exibePendencias&acao=A', 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');}</script>
<?=cabecalhoPDE(); ?>

<?php // INÍCIO - verifica se existem arquivos corrompidos da Escola ?>
<?php $arrWhere[] = "a.arqid not in(select arqid from public.arquivo_recuperado)"; ?>
<?php $arrWhere[] = "a.arqid/1000 between 647 and 725";?>
<?php $sql = "	SELECT
				      count(a.arqid) as arquivo
				FROM
					pdeescola.pdeescola pde
				INNER JOIN
				    pdeescola.planosuporteestrategico psepai ON psepai.pdeid = pde.pdeid
				INNER JOIN
					pdeescola.planosuporteestrategico pse ON pse.pseidpai = psepai.pseid
				INNER JOIN
					pdeescola.planosuporteestrategico psemeta ON psemeta.pseidpai = pse.pseid
				INNER JOIN
					pdeescola.planoacao pa ON pa.pseid = psemeta.pseid
				INNER JOIN
					pdeescola.detalheplanoacao dpa ON dpa.plaid = pa.plaid
				INNER JOIN
					pdeescola.monitoramentoacao mon ON mon.dpaid = dpa.dpaid
				INNER JOIN
					public.arquivo a ON a.arqid = mon.arqid
				WHERE
				      psepai.pseidpai IS NULL
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "" )."
				AND
					entid = $entid"; ?>
<?php $arrPerfil = pegaPerfilGeral() ?>
<?php $arrPerfil = !$arrPerfil ? array() : $arrPerfil;?>
<?php if($entid && $db->pegaUm($sql) >= 1 && ( in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL,$arrPerfil) || in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL,$arrPerfil) || $db->testa_superuser() ) ): ?>
	<?php $texto = "<div style=\"text-align:center;font-weight:bold;color:red;padding-top:10px;padding-bottom:10px;background-color:#F5F5F5\" >Atenção</div>"; ?>
	<?php $texto.= "<div style=\"font-weight:bold;padding-top:10px\" >Sr(a). Usuário(a),<br /><br />Existe(m) arquivo(s) da Escola, na aba de Monitoramento, que foram corrompidos. Por favor, substituir o(s) arquivo(s) marcado(s) em vermelho. <a href=\"pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A\" >Clique aqui</a> para ser direcionado(a) para a aba de Monitoramento.<br /><br/>Atenciosamente,<br/>Equipe – SIMEC - DTI</div>"; ?>
	<?php popupAlertaGeral($texto) ?>
<?php endif; ?>
<?php // FIM - verifica se existem arquivos corrompidos da Escola ?>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<?=montaTreeEscola();?>
			</td>
				<td style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left"> <?php	
					// Barra de estado atual e ações e Historico
					wf_desenhaBarraNavegacao( $docid , array( 'entid' => $_SESSION['entid']));?>
					</br>
						<?php if($estAtual['esdid'] == 37 or 
								 $estAtual['esdid'] == 38 or 
								 $estAtual['esdid'] == 61 or 
								 $estAtual['esdid'] == 76 or 
								 $estAtual['esdid'] == 86 or
								 $estAtual['esdid'] == 87					
							  ){
							 ?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
							<tr style="background-color: #c9c9c9; text-align:center;">
								<td style="font-size:7pt; text-align:center;">
									<span title="Cadastramento de parecer">
										<strong>Preenchimento</strong>
									</span>
								</td>
							</tr>
							<tr style="text-align:center;">
								<td style="font-size:7pt; text-align:center;">
			              			<a id="verificaPendencia" title="Verificar pendencias" onclick="verificaPendencia();">Verificar pendências</a>
								</td>
							</tr>
						</table> 
						<?php }else{?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
							<tr style="background-color: #c9c9c9; text-align:center;">
								<td style="font-size:7pt; text-align:center;">
									<span title="Cadastramento de parecer">
										<strong>Preenchimento</strong>
									</span>
								</td>
							</tr>
							<tr style="text-align:center;">
								<td style="font-size:7pt; text-align:center;">
			              			Plano Validado pelo MEC.
								</td>
							</tr>
						</table> 
						<?php }?>
					</td>
				</tr>
	</tbody>
</table>	
<!--<script type="text/javascript">-->
<?//if($pComite && $limComite && ($estAtual['esdid'] == 86)) {?>
<!--alert( 'Observações: \nÚltimo prazo para a tramitação do PDE-Escola do comitê para o MEC: 18/12/2009');-->
<?//}elseif($pEquipe && $limEquipe && ($estAtual['esdid'] == 76)){?>
<!--alert( 'Observações: \nÚltimo prazo para a tramitação do PDE da escola para o Comitê: 11/12/2009');-->
<?//}elseif($pEquipe && !$limEquipe && ($estAtual['esdid'] == 76)){?>
<!--alert( 'Observações: \nPrazo encerrado para a tramitação do PDE-Escola para o Comitê!');-->
<?//}elseif($pComite && !$limComite && ($estAtual['esdid'] == 86)){?>
<!--alert( 'Observações: \nPrazo encerrado para a tramitação do PDE-Escola para o MEC!' );-->
<?//}?>
<!--</script>-->
