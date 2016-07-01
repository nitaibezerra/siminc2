<?php
/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Gustavo Fernandes da Guarda
 */
 
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$titulo_modulo = "Associar professores a supervisores";

monta_titulo( $titulo_modulo, '' );

if($_GET['excluirTecidmonitorado'] && $_GET['excluirTecidsuper']){
	$oTecnicoMonitoraTecnico = new TecnicoMonitoraTecnico();
	$oTecnicoMonitoraTecnico->excluiProfessorPorTecidMonitorado($_GET['excluirTecidmonitorado'], $_GET['excluirTecidsuper']);
	$oTecnicoMonitoraTecnico->commit();
	unset($_GET['excluirTecidmonitorado'],$_GET['excluirTecidsuper']);
	unset($oTecnicoMonitoraTecnico);
	die("<script type=\"text/javascript\">alert('Operação Realizada com Sucesso.');window.location.href='escolaativa.php?modulo=principal/listaEstadoProfSupervisor&acao=A&estuf={$_GET['estuf']}';</script>");
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<!--<link rel="stylesheet" href="../includes/jquery-treeview/red-treeview.css" />
<link rel="stylesheet" href="screen.css" />
--><script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview-simec.js"></script>
<script type="text/javascript">
<!--

var janela;

$(document).ready(function(){
	
	$("#_arvore").treeview({
		collapsed: true,
		animated: "medium",
		control:"#sidetreecontrol",
		persist: "cookie"
	});

	$('#abrirTodos').click();

	$(window).unload(function() {
		janela.close();
	});

});

function associarProfessorSupervisor(tecid, estuf){
	janela = window.open('escolaativa.php?modulo=principal/popupAssociarProfessorSupervisor&acao=A&tecid='+tecid+'&estuf='+estuf,'AssociarProfessorSupervisor','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=900,height=700');
	janela.focus();
	return false;
}

function desassociarProfessorSupervisor( tecidmonitorado, tecidsuper ){
	if ( confirm( 'Deseja desassociar este professor?' ) ) {
		location.href= window.location+'&excluirTecidmonitorado='+tecidmonitorado+'&excluirTecidsuper='+tecidsuper;
	}
	return false;
}

//-->
</script>
<?php 
	$aMeses = array(1=>'Janeiro',
					2=>'Fevereiro',
					3=>'Março',
					4=>'Abril',
					5=>'Maio',
					6=>'Junho',
					7=>'Julho',
					8=>'Agosto',
					9=>'Setembro',
					10=>'Outubro',
					11=>'Novembro',
					12=>'Dezembro');
					
    extract($_GET);
    
    $tituloDiagnostico = "Diagnóstico Estadual";
    $sql = "select estdescricao from territorios.estado where estuf = '$estuf'";
    $estdescricao = $db->pegaUm($sql);
    
    $sql = "select 
	    distinct
	    t.tecid,
	    e.entid,
	    e.entnome
	from escolaativa.tecnico t
	    inner join entidade.entidade e on t.entid = e.entid
	    inner join escolaativa.usuarioresponsabilidade ur on e.entnumcpfcnpj = ur.usucpf and ur.rpustatus = 'A'
	where ur.pflcod = ".ESCOLAATIVA_PERFIL_SUPERVISOR."
	and ur.estuf = '$estuf'
	";
    $aSupervisor = $db->carregar($sql);
    $aSupervisor = ($aSupervisor) ? $aSupervisor : array();
    
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;" id="_arvore">
			<div id="main" style="background: #f5f5f5">
				<div id="sidetree">
					<div class="treeheader">&nbsp;</div>
					<div id="sidetreecontrol"><a href="?#">Fechar Todos</a> | <a href="?#" id="abrirTodos">Abrir Todos</a> <br /><br /> <img src="../includes/jquery-treeview/images/base.gif" align="top" /><?php echo $tituloDiagnostico; ?></div>
						<ul id="tree" class="filetree treeview-famfamfam">
							<li><span class="folder"><?php echo $estdescricao; ?></span>
								<ul>
									<li><span class="folder">Supervisores</span>
										<ul>
											<?php if(count($aSupervisor) && $aSupervisor[0]): ?>
												<?php foreach($aSupervisor as $supervisor) : ?>
													<li>
														<span><img alt="" src="../imagens/gif_inclui.gif" title="Associar Escola" style="cursor: pointer" onclick="associarProfessorSupervisor('<?php echo $supervisor['tecid']; ?>', '<?php echo $estuf;?>')" align="top" border="0" /> <img alt="" src="../imagens/pessoas.png" align="top" border="0" /> <?php echo $supervisor['entnome']; ?></span>
														<?php 
															$sql = "select 
																	    distinct
																	    t.tecid,
																	    e.entid,
																	    e.entnome
																	from escolaativa.tecnico t
																	    inner join entidade.entidade e on t.entid = e.entid    
																	    inner join escolaativa.tecnicoresponsabilidade tr on t.tecid = tr.tecid
																	    inner join escolaativa.usuarioresponsabilidade ur on e.entnumcpfcnpj = ur.usucpf and ur.rpustatus = 'A'
																	    inner join escolaativa.tecnicomonitoratecnico tmt on t.tecid = tmt.tecidmonitorado
																	    inner join territorios.municipio m on ur.muncod = tr.muncod
																	where 
																	m.estuf = '$estuf' and 
																	ur.pflcod = ".ESCOLAATIVA_PERFIL_MULTIPLICADOR." and
																	tmt.tecidsuper = {$supervisor['tecid']}";
														    $arProfessorAssociados = $db->carregar($sql);
														    $arProfessorAssociados = ($arProfessorAssociados) ? $arProfessorAssociados : array();
														?>
														<ul>
															<?php if(count($arProfessorAssociados) && $arProfessorAssociados[0]): ?>
																<?php foreach($arProfessorAssociados as $professorAssociado) : ?>
																	<li><span> <img alt="" src="../imagens/exclui_p.gif" title="Desassociar Escola" style="cursor: pointer" onclick="desassociarProfessorSupervisor('<?php echo $professorAssociado['tecid']; ?>', '<?php echo $supervisor['tecid']; ?>')" align="top" border="0" /> <?php echo $professorAssociado['entnome']; ?></span></li>
																<?php endforeach;?>
															<?php else: ?>
																	<li><span class="file"> Não existe professor associado, para associar <a href="#" onclick="associarProfessorSupervisor('<?php echo $supervisor['tecid']; ?>', '<?php echo $estuf;?>')" >clique aqui.</a></span></li>
															<?php endif;?>
														</ul>
													</li>
												<?php endforeach;?>
											<?php endif; ?>
											<!--<li><span><a href="par.php?modulo=principal/questoesPontuais&acao=A"><img alt="Questões Pontuais" src="../includes/jquery-treeview/images/question.gif" align="top" border="0" /><strong>Questões Pontuais</strong></a></span></li>-->
										</ul>
									</li>
								</ul>
							</li>
						</ul> <!-- ul #tree -->
				</div><!-- div #sidetree -->
			</div><!-- div #main -->
		</td>
	</tr>
</table>