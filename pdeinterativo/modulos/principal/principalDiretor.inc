<?php
if($_POST['requisicao']){
	$arrDados = $_POST['requisicao']();
}

if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}

$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);
$_SESSION['pdeinterativo_vars']['pdicodinep'] = $arrDados['pdicodinep'];
$_SESSION['pdeinterativo_vars']['pdeid'] = $arrDados['pdeid'];
$_SESSION['pdeinterativo_vars']['pdenome'] = $arrDados['pdenome'];

if(!$_SESSION['pdeinterativo_vars']['pdicodinep']){
	echo "<script>alert('Escola não encontrada!');history.back(-1);</script>";
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview.js"></script>
<script type="text/javascript" src="../pdeinterativo/js/pdeinterativo.js"></script>
<script>	
	
	jQuery(function() {
		jQuery("#treepdeinterativo").treeview({
			collapsed: false,
			animated: "medium",
			persist: "cookie"
	
		});
		<?php if($_SESSION['pdeinterativo']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo']['msg']) ?>
		<?php endif; ?>
	});
	
</script>

<style>
	#treepdeinterativo span {cursor:pointer};
</style>

<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>
<?php cabecalhoPDEInterativo($arrDados); ?>
<?php $arrAbasOK = $db->carregarColuna("select abaid from pdeinterativo.abaresposta where pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}") ?>
<?php $arrAbasOK = !$arrAbasOK ? array() : $arrAbasOK ?>
<?php barraProgressoPDEInterativo(); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="70%" valign="top" >
			<ul id="treepdeinterativo" class="filetree" >
				<li><span class="folder bold">&nbsp;PDE Interativo</span>
					<ul>
						<li><span class="folder">&nbsp;Identificação</span>
							<ul><li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Diretor'" class="file">&nbsp;Identificação do(a) Diretor(a) <img class="img_middle" src="<?php echo in_array(42,$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png" ?>" /></span></li></ul>
							<ul><li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola'" class="file">&nbsp;Identificação da Escola <img class="img_middle" src="<?php echo in_array(43,$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png" ?>" /> </span></li></ul>
							<ul><li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Galeria'" class="file">&nbsp;Galeria de fotos <img class="img_middle" src="<?php echo in_array(56,$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png" ?>" /> </span></li></ul>
						</li>
					</ul>
					<ul>
						<li><span class="folder">&nbsp;Primeiros Passos</span>
							<ul><li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso'" class="file">&nbsp;Passo 1 <img class="img_middle" src="<?php echo in_array(46,$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png" ?>" /></span></li></ul>
							<ul><li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=segundopasso'" class="file">&nbsp;Passo 2 <img class="img_middle" src="<?php echo in_array(48,$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png" ?>" /></span></li></ul>
							<ul><li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=terceiropasso'" class="file">&nbsp;Passo 3 <img class="img_middle" src="<?php echo in_array(49,$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png" ?>" /></span></li></ul>
						</li>
					</ul>
					<ul><li><span class="folder">&nbsp;Diagnóstico</span>
					<?php $arrModulos = $db->carregar("select * from pdeinterativo.aba where abaidpai = 1 and abatipo is null order by abaid") ?>
					<?php if($arrModulos): ?>
						<?php foreach($arrModulos as $mod): ?>
							<ul>
								<li><span class="folder">&nbsp;<?php echo $mod['abadescricao'] ?></span>
									<?php $arrSubModulos = $db->carregar("select * from pdeinterativo.aba where abaidpai = {$mod['abaid']} and (abatipo is null OR abatipo='S') order by abaid") ?>
									<?php if($arrSubModulos): ?>
										<?php foreach($arrSubModulos as $sub): ?>
											<ul>
												<li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=<?php echo $mod['abacod'] ?>&aba1=<?php echo $sub['abacod'] ?>'" class="file">&nbsp;<?php echo $sub['abadescricao'] ?> <img class="img_middle" src="<?php echo $sub['abatipo'] != "O" ? (in_array($sub['abaid'],$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png") : "../imagens/check_p.gif" ?>" /></span></li>
											</ul>
										<?php endforeach; ?>
									<?php endif; ?>
								</li>
							</ul>
						<?php endforeach; ?>
					<?php endif; ?>
					</li>
					<li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese'" class="file">&nbsp;7. Síntese do diagnóstico <img class="img_middle" src="<?php echo in_array(9,$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png" ?>" /> </span></li>
					
					<ul><li><span class="folder">&nbsp;Plano Geral</span>
					<?php $arrModulos = $db->carregar("select * from pdeinterativo.aba where abaidpai = 50 and abatipo is null order by abaid") ?>
					<?php if($arrModulos): ?>
						<?php foreach($arrModulos as $mod): ?>
							<ul>
								<li><span class="folder">&nbsp;<?php echo $mod['abadescricao'] ?></span>
									<?php $arrSubModulos = $db->carregar("select * from pdeinterativo.aba where abaidpai = {$mod['abaid']} and (abatipo is null OR abatipo='S') order by abaid") ?>
									<?php if($arrSubModulos): ?>
										<?php foreach($arrSubModulos as $sub): ?>
											<ul>
												<li><span onclick="window.location.href='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=<?php echo $mod['abacod'] ?>&aba1=<?php echo $sub['abacod'] ?>'" class="file">&nbsp;<?php echo $sub['abadescricao'] ?> <img class="img_middle" src="<?php echo $sub['abatipo'] != "O" ? (in_array($sub['abaid'],$arrAbasOK) ? "../imagens/check_p.gif" : "../imagens/atencao.png") : "../imagens/check_p.gif" ?>" /></span></li>
											</ul>
										<?php endforeach; ?>
									<?php endif; ?>
								</li>
							</ul>
						<?php endforeach; ?>
					<?php endif; ?>
					</li>
			</ul>
		</td>
		<td valign="top" align="center" >
			<table class="tabela" width="100%" align="center">
				<tr>
					<td style="padding:5px" align="center" class="bold" bgcolor="#FFFFFF" >O que deseja fazer agora?</td>
				</tr>
				<tr>
					<td style="padding:5px" align="center" bgcolor="#FFFFFF" >
						<input type="button" name="btn_iniciar_plano" value="Iniciar Plano" <?php echo count($arrAbasOK) > 0 ? "disabled='disabled'": "onclick=\"window.location.href='pdeinterativo.php?modulo=principal/identificacao&acao=A'\"" ?> />
						<?php
						if(count($arrAbasOK) > 0 ){
							if(!in_array(42,$arrAbasOK)){
								$abacodContinuar = "pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Diretor";
							}elseif(!in_array(43,$arrAbasOK)){
								$abacodContinuar = "pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola";
							}elseif(!in_array(46,$arrAbasOK)){
								$abacodContinuar = "pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso";
							}elseif(!in_array(48,$arrAbasOK)){
								$abacodContinuar = "pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=segundopasso";
							}elseif(!in_array(49,$arrAbasOK)){
								$abacodContinuar = "pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=terceiropasso";
							}else{
								$sql = "
								select 
									abacod as abafilho,
									( 
										select 
											abacod 
										from
											pdeinterativo.aba aba1
										where
											aba1.abaid = aba2.abaidpai
									) as abapai
								from
									pdeinterativo.aba aba2
								where 
									aba2.abaid not in (select distinct abaid from pdeinterativo.abaresposta where pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']})
								and
									aba2.abaid not in (9)
								and
									aba2.abaidpai is not null
								and
									aba2.abatipo != 'O'
								order by
									aba2.abaid
								limit 1;";

								$aba = $db->pegaLinha($sql);
								if($aba){
									$abacodContinuar = "pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba={$aba['abapai']}&aba1={$aba['abafilho']}";
								}
								
								if(!$abacodContinuar){
									if(!in_array(9,$arrAbasOK)){
										$abacodContinuar = "pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese";
									}else{
										$abacodContinuar = false;
									}
								}
							}
						}
						
						?>
						<input type="button" name="btn_continuar_plano" value="Continuar Plano" <?php echo count($arrAbasOK) == 0 || $abacodContinuar == false? "disabled='disabled'": "onclick=\"window.location.href='$abacodContinuar'\"" ?> />
						<input type="button" name="btn_visualizar_plano" value="Visualizar Plano" <?php echo count($arrAbasOK) == 0 ? "disabled='disabled'": "onclick=\"window.location.href='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_3_visualizarplanoacao';\"" ?> />
					</td>
				</tr>
			</table>
			<table style="margin-top:15px" class="tabela" bgcolor="#FFFFFF" width="100%" align="center">
				<tr>
					<td colspan="2" style="padding:5px" align="center" class="bold" >Situação do PDE da Escola <?php echo $_SESSION['pdeinterativo_vars']['pdenome'] ?></td>
				</tr>
				<tr>
					<td width="25%" class="bold direita" >Fase Atual:</td>
					<td class="esquerda" >
						<?php 
							if( count($arrAbasOK) == 0){
								echo "<b>Não Iniciado</b>";
							}
							if( count($arrAbasOK) > 0){
								
								$sql = "SELECT esd.esddsc,now() - h.htddata as htddata, d.docid, esd.esdid
										FROM pdeinterativo.pdinterativo p 
										LEFT JOIN workflow.documento d ON d.docid = p.docid
										LEFT JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
										LEFT JOIN workflow.historicodocumento h ON d.docid = h.docid  
										WHERE p.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' order by h.htddata desc";
								$arrDadosPDE = $db->pegaLinha($sql);
								$esddsc = $arrDadosPDE['esddsc'];
								$htddata = $arrDadosPDE['htddata'];
								
								if($esddsc){
									echo "<img border=0 align=absmiddle src=../imagens/editar_nome_vermelho.gif style=cursor:pointer; onclick=\"wf_exibirHistorico('".$arrDadosPDE['docid']."');\"> <b>$esddsc</b>";
								}else{
									echo "<b>Em Elaboração</b>";	
								}
							}
						
							if(!$abacodContinuar && count($arrAbasOK) > 0){
								$sql = "select now() - abrdata from pdeinterativo.abaresposta where abaid = 9 and pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}";
								$data = $db->pegaUm($sql);
								if(!$htddata && $data){
									if(strstr($data,"day")){
										$arrData = explode("day",$data);
										echo " - Há {$arrData[0]} dia(s).";
									}
								}else{
									if(strstr($htddata,"day")){
										$arrData = explode("day",$htddata);
										echo " - Há {$arrData[0]} dia(s).";
									}
								}
							}else{
								$sql = "select now() - abrdata from pdeinterativo.abaresposta where pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']} order by abrdata limit 1;";
								$data = $db->pegaUm($sql);
								if(!$htddata && $data){
									if(strstr($data,"day")){
										$arrData = explode("day",$data);
										echo " - Há {$arrData[0]} dia(s).";
									}
								}else{
									if(strstr($htddata,"day")){
										$arrData = explode("day",$htddata);
										echo " - Há {$arrData[0]} dia(s).";
									}
								}
							}
							
						?>
						
					</td>
				</tr>
				<?
				
				// Trecho que exibe os pareceres retornados para a escola -> comite para escola - Alexandre
				if($arrDadosPDE['docid'] && $arrDadosPDE['esdid']==WF_ESD_ELABORACAO) {
					
					echo "<tr><td colspan=2><div style=height:200px;overflow:auto;>";
					
					$sql = "SELECT * FROM workflow.historicodocumento hst 
							INNER JOIN workflow.comentariodocumento chd ON chd.hstid = hst.hstid  
							WHERE aedid='".WF_AED_DEVOLVER_COMITE_MEC."' AND hst.docid='".$arrDadosPDE['docid']."' ORDER BY htddata DESC LIMIT 1";
					$historicodocumento = $db->pegaLinha($sql);
					
					if($historicodocumento) {
						
						// Pegando todos
						$html .= "<table class=listagem cellSpacing=1 cellPadding=3 align=center width=100%>";
						$html .= "<tr>";
						$html .= "<td class=SubTituloDireita>Parecer Diagnóstico</td>";
						$sql = "SELECT a.abadescricao, p.prcparecer FROM pdeinterativo.parecer p 
								INNER JOIN pdeinterativo.aba a ON a.abaid = p.abaid 
								WHERE p.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND p.prcstatus='A' AND p.prcparecer IS NOT NULL ORDER BY a.abaid";
						$parecediag = $db->carregar($sql);
						if($parecediag[0]) {
							foreach($parecediag as $pardiag) {
								$_diagnostico[$pardiag['abadescricao']][] = $pardiag['prcparecer']; 
							}
						}
						
						$html .= "<td><p>O Diagnóstico deverá ser ajustado de acordo com os seguintes comentários:</p>";
						
						if($_diagnostico) {
							foreach($_diagnostico as $abadescricao => $pareceres) {
								$html .= "<b>".$abadescricao."</b><br>";
								foreach($pareceres as $parecer) {
									$html .= "- ".$parecer."<br>";
								}
							}
						}
						unset($_diagnostico);
						
						$html .= "</td>";
						$html .= "</tr>";
						$html .= "<tr>";
						$html .= "<td class=SubTituloDireita>Parecer Planos de Ação</td>";
						$html .= "<td><p>Os itens abaixo precisam ser revistos (valores, quantidades e/ou o próprio item):</p>";
						
						$sql = "select ps.pabqtd||' '||ci.ciadesc as item, abadescricao as dimensao from pdeinterativo.planoacaoproblema pa 
								inner join pdeinterativo.aba ab on ab.abacod=pa.abacod
								inner join pdeinterativo.planoacaoestrategia pe on pe.papid=pa.papid 
								inner join pdeinterativo.planoacaoacao pc on pc.paeid=pe.paeid 
								inner join pdeinterativo.planoacaobemservico ps on ps.paaid=pc.paaid 
								inner join pdeinterativo.categoriaitemacao ci on ci.ciaid=ps.ciaid
								where pa.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' and 
									  pa.papstatus='A' and 
									  pe.paestatus='A' AND 
									  pc.paastatus='A' AND 
									  ps.pabstatus='A' AND 
									  (ps.pabcomiteanalise IS NULL OR ps.pabcomiteanalise=FALSE)";
				
						$placao = $db->carregar($sql);
						
						if($placao[0]) {
							foreach($placao as $pa) {
								$_diagnostico[$pa['dimensao']][] = $pa['item'];
							}
						}
						
						if($_diagnostico) {
							foreach($_diagnostico as $abadescricao => $pareceres) {
								$html .= "<b>".$abadescricao."</b><br>";
								foreach($pareceres as $parecer) {
									$html .= "- ".$parecer."<br>";
								}
							}
						}
						unset($_diagnostico);
						
						$html .= "<p>Em caso de dúvida, procure o Comitê de Análise e Aprovação da sua Secretaria.</p></td>";
						
						$html .= "</tr>";
						$html .= "<tr><td class=SubTituloDireita>Comentários</td><td>".$historicodocumento['cmddsc']."</td></tr>";
						
						$html .= "</table>";
						
						echo $html;
					}
					
					echo "</div></td></tr>";
					
				}
				// FIM - Trecho que exibe os pareceres retornados para a escola -> comite para escola - Alexandre
				
				
				
				?>
				<?php if($esddsc == "Validado pelo MEC"): ?>
					<?php 
					
					$sql = "select 
								(CASE 
									WHEN spasituacao is true THEN 'Pago' 
									WHEN spasituacao is false THEN 'Pendente'
									WHEN spasituacao is null THEN 'N/A'
								END) as pagamento
							from
								pdeinterativo.situacaopagamento
							where
								pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}";
					$pagamento = $db->pegaUm($sql);
					
					?>
					<tr>
						<td class="bold direita" >Situação PG:</td>
						<td class="esquerda bold" ><?php echo (($pagamento)?$pagamento:"N/A") ?></td>
					</tr>
				<?php endif; ?>
				
				<tr>
					<td colspan="2" style="padding: 10 10 10 10">
					<p align="center"><font color="red"><b>AVISO IMPORTANTE:</b></font></p>
					<p>A partir de 2012, o PDE Interativo será o sistema utilizado pelas escolas públicas para elaboração do Plano de Formação Continuada dos Profissionais da Educação Básica. Assim, todas as escolas que desejem indicar profissionais para participar de cursos de formação continuada oferecidos pelo MEC em 2013 devem elaborar o seu Plano de Formação no PDE Interativo.</p>
					<p>Embora seja recomendável, não é obrigatório concluir todo o Diagnóstico do PDE Interativo antes de enviar o Plano de Formação. É suficiente que a escola faça os "Primeiros Passos" e finalize as atividades das telas "4.1 Direção" e "5.2 Docentes". A definição dos “Grandes desafios” e a elaboração dos “Planos de Ação” também é opcional antes do envio do Plano de Formação.</p>
					<p>Caso deseje, após enviar o Plano de Formação Continuada, a escola pode continuar utilizando as demais funcionalidades do PDE Interativo, debatendo outros aspectos da gestão junto à comunidade escolar. E no caso das escolas priorizadas pelo PDE Escola, para o recebimento dos recursos é obrigatório concluir todo o planejamento, inclusive os "Grandes desafios" e "Planos de ação".</p>
					<p>Para saber mais, acesse o site do PDE Escola: <a href="http://pdeescola.mec.gov.br" target="_blank">pdeescola.mec.gov.br</a>, ligue para 0800 616161 (opção 6, sub-opção 1) ou envie um e-mail para pdeinterativo@mec.gov.br. </p> 
					</td>
				</tr>    
			</table>
		</td>
	</tr>
</table>