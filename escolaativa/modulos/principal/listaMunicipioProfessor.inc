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

$titulo_modulo = "Associar escolas a professores";

monta_titulo( $titulo_modulo, '' );

if($_GET['excluirEspid'] && $_GET['excluirTecid']){
	$oTecnicoMonitoraEscola = new TecnicoMonitoraEscola();
	$oTecnicoMonitoraEscola->excluiMonitoriaEscolaPorEspid($_GET['excluirEspid'], $_GET['excluirTecid']);
	$oTecnicoMonitoraEscola->commit();
	unset($_GET['excluirEspid'],$_GET['excluirTecid']);
	unset($oTecnicoMonitoraEscola);
	die("<script type=\"text/javascript\">alert('Operação Realizada com Sucesso.');window.location.href='escolaativa.php?modulo=principal/listaMunicipioProfessor&acao=A&muncod={$_GET['muncod']}';</script>");
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

function associarEscolaProfessor(tecid, muncod){
	janela = window.open('escolaativa.php?modulo=principal/popupAssociarEscolaProfessor&acao=A&tecid='+tecid+'&muncod='+muncod,'AssociarEscolaProfessor','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=900,height=700');
	janela.focus();
	return false;
}

function excluirEscolaMunicipio( espid, tecid ){
	if ( confirm( 'Deseja desassociar esta escola?' ) ) {
		location.href= window.location+'&excluirEspid='+espid+'&excluirTecid='+tecid;
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
    
    $tituloDiagnostico = "Diagnóstico Municípal";
    $sql = "select mundescricao, e.estdescricao from territorios.municipio m
				inner join territorios.estado e on m.estuf = e.estuf
			where muncod = '$muncod'";
    $aMunicipio = $db->pegaLinha($sql);
    
    $munestdescricao = $aMunicipio['mundescricao'];
    $estdescricao    = $aMunicipio['estdescricao'];
    
   	$sql = "select 
   				distinct
    			t.tecid,
    			e.entid,
    			e.entnome
			from escolaativa.tecnico t
				inner join entidade.entidade e on t.entid = e.entid
				inner join escolaativa.tecnicoresponsabilidade tr on t.tecid = tr.tecid
				inner join escolaativa.usuarioresponsabilidade ur on e.entnumcpfcnpj = ur.usucpf and ur.rpustatus = 'A'
	    	where tr.muncod = '$muncod' 
	    	and ur.pflcod = ".ESCOLAATIVA_PERFIL_MULTIPLICADOR;
    $aProfessorMultiplicador = $db->carregar($sql);
    $aProfessorMultiplicador = ($aProfessorMultiplicador) ? $aProfessorMultiplicador : array();
    
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;" id="_arvore">
			<div id="main" style="background: #f5f5f5">
				<div id="sidetree">
					<div class="treeheader">&nbsp;</div>
					<div id="sidetreecontrol"><a href="?#">Fechar Todos</a> | <a href="?#" id="abrirTodos">Abrir Todos</a> <br /><br /> <img src="../includes/jquery-treeview/images/base.gif" align="top" /><?php echo $tituloDiagnostico; ?></div>
						<ul id="tree" class="filetree treeview-famfamfam">
							<li><span class="folder"><?php echo $munestdescricao; ?></span>
								<ul>
									<li><span class="folder">Professores</span>
										<ul>
											<?php if(count($aProfessorMultiplicador) && $aProfessorMultiplicador[0]): ?>
												<?php foreach($aProfessorMultiplicador as $professorMultiplicador) : ?>
													<li>
														<span><img alt="" src="../imagens/gif_inclui.gif" title="Associar Escola" style="cursor: pointer" onclick="associarEscolaProfessor('<?php echo $professorMultiplicador['tecid']; ?>', '<?php echo $muncod;?>')" align="top" border="0" /> <img alt="" src="../imagens/pessoas.png" align="top" border="0" /> <?php echo $professorMultiplicador['entnome']; ?></span>
														<?php 
															$sql = "select ep.espid, e.entnome from cte.escolaativa ea
																		inner join cte.instrumentounidade inu on inu.inuid = ea.inuid 
																		inner join cte.escolaativaescolas eae on ea.esaid = eae.esaid
																		inner join entidade.entidade e on eae.entid = e.entid
																		inner join escolaativa.escolaparticipante ep on eae.eaeid = ep.eaeid
																		inner join escolaativa.tecnicomonitoraescola tme on ep.espid = tme.espid
																	where inu.muncod = '$muncod' AND tme.tecid = {$professorMultiplicador['tecid']}
																	order by e.entnome";
															$aEscolaTecnico = $db->carregar($sql);
															$aEscolaTecnico = ($aEscolaTecnico) ? $aEscolaTecnico : array();
														?>
														<ul>
															<?php if(count($aEscolaTecnico) && $aEscolaTecnico[0]): ?>
																<?php foreach($aEscolaTecnico as $escolaTecnico) : ?>
																	<li><span> <img src="../imagens/exclui_p.gif" title="Desassociar Escola" style="cursor: pointer" onclick="excluirEscolaMunicipio('<?php echo $escolaTecnico['espid']; ?>', '<?php echo $professorMultiplicador['tecid']; ?>')" align="top" border="0" /> <?php echo $escolaTecnico['entnome']; ?></span></li>
																<?php endforeach;?>
															<?php else: ?>
																	<li><span class="file"> Não existe escola associada, para associar <a href="#" onclick="associarEscolaProfessor('<?php echo $professorMultiplicador['tecid']; ?>', '<?php echo $muncod;?>')" >clique aqui.</a></span></li>
															<?php endif;?>
														</ul>
													</li>
												<?php endforeach;?>
											<?php else: ?>
													<li><span class="file"> Não existe professor multiplicador para associar a este município.</span></li>
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