<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	

<?php if(in_array(PERFIL_SUPER_USUARIO,$perfis) || in_array(PERFIL_ADMINISTRADOR,$perfis)){  ?>
<form id="formulario" method="post" name="formulario" action="">
	<input type="hidden" id="requisicao" name="requisicao" value=""/>
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
		<tr>
			<td align='right' width="30%" class="SubTituloDireita">IES:</td>
			<td>
				<?php 
				if($co_ies){
					$sql = "SELECT 	co_ies as codigo,
									no_ies as descricao
							FROM	emec.ies
							WHERE	co_ies = {$co_ies}";
					$resp = $db->carregar($sql);
				}
				
				$sql = "SELECT 		co_ies AS codigo, 
									no_ies AS descricao
	  					FROM 		emec.ies
	  					WHERE		1 = 1";
				
				combo_popup('co_ies', $sql, 'IES', '400x400', '0',
							'', '', 'S', '', '', 2, 400 , $onpop = null, $onpush = null, 
							$param_conexao = false, Array(Array('codigo' => 'no_ies', 'descricao' => 'IES')), 
							$resp, $mostraPesquisa = true, $campo_busca_descricao = false, 
							"exibirCurso(this);", $intervalo=false, $arrVisivel = null , $arrOrdem = null); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Curso:</td>
			<td id="divCursos">
	        	<?php $sql = "SELECT DISTINCT co_curso AS codigo, no_curso AS descricao FROM emec.cursos ORDER BY no_curso";
	        	$db->monta_combo('co_curso',$sql,'S','Selecione um curso','','','','372','S','',''); ?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita">Habilitação:</td>
			<td>
				<?php $sql = "SELECT DISTINCT habid as codigo, habdesc as descricao FROM assint.habilitacao	WHERE habstatus = 'A' ORDER BY habdesc ASC";
				$db->monta_combo("habid",$sql,'S',"Selecione uma habilitação",'','','','372','S','','');	?>
			</td>
		</tr>			
		<tr>
			<td class="subtituloDireita">Turno:</td>
			<td>
				<?php 
					$aryTurno = array(
						array('codigo'=>'M', 'descricao'=>'Matutino'),
						array('codigo'=>'V', 'descricao'=>'Vespertino'),
						array('codigo'=>'I', 'descricao'=>'Integral'));
					$db->monta_combo('ofpturno', $aryTurno, 'S', 'Selecione o turno', '', '','','200','S','',''); ?>		
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#DCDCDC" colspan="2">
				<input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisarCurso();"/>
				<input type="button" value="Gerar Excel" id="btnGerarExcel" onclick="gerarExcel();"/>
			</td>
		</tr>		
	</table>
</form>
<?php } ?>

<br>

<?php if(in_array(PERFIL_SUPER_USUARIO,$perfis) || in_array(PERFIL_ADMINISTRADOR,$perfis) || in_array(PERFIL_PEC_G,$perfis)){  ?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td class="SubTituloEsquerda" colspan="2">
      		<span class="adicionar" style="cursor:pointer"><img border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif">&nbsp;Inserir Novo Curso</span>
    	</td>
    </tr>
</table>
<?php } ?>

<div id="divCurso"><?php pesquisarCurso($co_ies_filtro); ?></div>