<?php
require_once APPRAIZ . 'includes/classes/consultaReceitaFederal.class.inc';

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}

include  APPRAIZ."includes/cabecalho.inc";
$arrPerfil = pegaPerfilGeral();
?>
<br />
<?php $db->cria_aba($abacod_tela,$url,''); ?>
<?php monta_titulo( 'Dados da Instituição de Ensino' , obrigatorio()." Indica campos obrigatórios.");  ?>

<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function aprovarAdesao()
	{
		jQuery("[name='requisicao']").val("aprovarAdesao");
		jQuery("[name='formulario_comite']").submit();
	}
	
	function rejeitarAdesao()
	{
		jQuery("[name='requisicao']").val("rejeitarAdesao");
		jQuery("[name='formulario_comite']").submit();
	}
</script>
<?php $arrDados = pegaDadosInstituicaoPorId($_GET['insid']) ?>
<form name="formulario_comite" id="formulario_comite"  method="post" action="" enctype="multipart/form-data" >
	<input type="hidden"  name="requisicao" value="" />
	<input type="hidden"  name="insid" value="<?php echo $arrDados['insid'] ?>" />
	<input type="hidden"  name="dirid" value="<?php echo $arrDados['dirid'] ?>" />
	<table id="tabela_comite" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2" class="SubtituloTabela bold" >Dados da Instituição</td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita" >Nome:</td>
			<td>
				<?php echo $arrDados['insnome'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Sigla:</td>
			<td>
				<?php echo $arrDados['inssigla'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >CNPJ:</td>
			<td>
				<?php echo $arrDados['inscnpj'] ? mascaraglobalTermoAdesao($arrDados['inscnpj'],"##.###.###/####-##") : "" ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Telefone:</td>
			<td>
				<?php echo $arrDados['instelefone'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Fax:</td>
			<td>
				<?php echo $arrDados['insfax'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >E-mail:</td>
			<td>
				<?php echo $arrDados['insemail'] ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubtituloTabela bold" >Endereço</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >CEP:</td>
			<td>
				<input type="hidden" name="endid" value="<?php echo $arrDados['endid'] ?>" />
				<?php echo $arrDados['endcep'] ? mascaraglobalTermoAdesao($arrDados['endcep'],"##.###-###") : "" ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Logradouro:</td>
			<td>
				<?php echo $arrDados['endlog'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Complemento:</td>
			<td>
				<?php echo $arrDados['endcom'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Número:</td>
			<td>
				<?php echo $arrDados['endnum'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Bairro:</td>
			<td>
				<?php echo $arrDados['endbai'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Estado:</td>
			<td>
				<?php $sqlUF = "	select 
										estdescricao as descricao 
									from 
										territorios.estado
									where
										estuf = '{$arrDados['estuf']}'
									order by 
										estdescricao"; ?>
				<?php echo $db->pegaUm($sqlUF) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Município:</td>
			<td id="td_muncod_endereco" >
			<?php $sqlMun = "	select 
										mundescricao as descricao 
									from 
										territorios.municipio
									where
										muncod = '{$arrDados['muncod']}'
									order by 
										mundescricao";?>

				<?php echo $db->pegaUm($sqlMun) ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubtituloTabela bold" >Dados do Dirigente</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Nome:</td>
			<td><?php echo $arrDados['dirnome'] ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >CPF:</td>
			<td><?php echo $arrDados['dircpf'] ? mascaraglobalTermoAdesao($arrDados['dircpf'],"###.###.###-##") : "N/A" ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Cargo:</td>
			<td>
				<?php $sqlCargo = "	select 
										cardescricao as descricao 
									from 
										snf.cargo 
									where 
										carstatus = 'A'
									and
										carid = {$arrDados['carid']}
									order by 
										cardescricao"; ?>
				<?php echo $db->pegaUm($sqlCargo) ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubtituloTabela bold" >Dados do Comitê Institucional</td>
		</tr>
		<tr class="SubtituloCentro" >
			<td colspan="2" class="bold" >Coordenador Institucional</td>
		</tr>
		<?php 
			$sql = "	select
							*
						from
							snf.comite com
						left join
							snf.anexo anx ON anx.aneid = com.aneid and anestatus = 'A'
						left join
							public.arquivo arq ON anx.arqid = arq.arqid
						left join
							snf.membrocomite mem ON com.comid = mem.comid and memstatus = 'A'
						where
							com.insid = {$arrDados['insid']}
						and
							comstatus = 'A'";
			$arrDados = $db->carregar($sql);
			if($arrDados){
				foreach($arrDados as $dado){
					if($dado['papid'] == PAPEL_COORD_INSTITUCIONAL){
						$arrCood = $dado;
					}else{
						$arrMembros[] = $dado;
					}
				}
			}
		?>
		<tr>
			<td class="SubtituloDireita" width="25%">
				CPF:
			</td>
			<td>
				<input type="hidden"  name="papid[]" value="<?php echo PAPEL_COORD_INSTITUCIONAL ?>" />
				<?php echo $arrCood['memcpf'] ? mascaraglobalTermoAdesao($arrCood['memcpf'],"###.###.###-##") : "N/A" ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Nome:
			</td>
			<td>
				<?php echo $arrCood['memnome'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Email:
			</td>
			<td>
				<?php echo $arrCood['mememail'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Telefone:
			</td>
			<td>
				<?php echo $arrCood['memtelefone'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Endereço:
			</td>
			<td>
				<?php echo $arrCood['memendereco'] ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Estado:
			</td>
			<td>
				<?php $sqlUF = "	select 
										estdescricao as descricao 
									from 
										territorios.estado
									where
										estuf = '{$arrCood['estuf']}'
									order by 
										estdescricao"; ?>
				<?php echo $db->pegaUm($sqlUF) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Município:
			</td>
			<td id="td_muncod" >
				<?php $sqlMun = "	select 
										mundescricao as descricao 
									from 
										territorios.municipio
									where
										muncod = '{$arrCood['muncod']}'
									order by 
										mundescricao";?>

				<?php echo $db->pegaUm($sqlMun) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Ato Constitutivo:
			</td>
			<td>
				<?php if($arrCood['arqid']): ?>
					<div id="div_file_link" >
						<a href='javascript:downloadArquivo(<?php echo $arrCood['arqid'] ?>)' ><?php echo $arrCood['arqnome'] ?>.<?php echo $arrCood['arqextensao'] ?></a> <img src='../imagens/excluir.gif' onclick='excluirArquivoComite(<?php echo $arrCood['arqid'] ?>)' style='vertical-align:middle;cursor:pointer' />
						<input type="hidden" name="hdn_arquivo_comite" value="1"  />
					</div>
					<?php $ocultaFile = true ?>
				<?php endif; ?>
				<div id="div_file_upload" style="display:<?php echo $ocultaFile ? "none" : "" ?>" >
					<input type="file" name="arquivo_comite"  /> <?php echo obrigatorio() ?><span style="padding-left:10px" >* Documento de formalização da criação do comitê.</span>
				</div>
			</td>
		</tr>
		<tr class="SubtituloCentro" >
			<td colspan="2" class="bold" >Demais Membros do Comitê</td>
		</tr>
		<?php if($arrMembros): ?>
			<?php $n=2; ?>
			<?php foreach($arrMembros as $membro): ?>
				<tr class="SubtituloTabela" >
					<td colspan="2" >
						Membro <?php echo ($n -1) ?> <img onclick="excluiMembroComite(this)" style="background-color:#FFFFFF" class="img_middle link" src="../imagens/excluir.gif">
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita">
						CPF:
					</td>
					<td>
						<?php echo $membro['memcpf'] ? mascaraglobalTermoAdesao($membro['memcpf'],"###.###.###-##") : "N/A" ?>
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita" >
						Nome:
					</td>
					<td>
						<?php echo $membro['memnome'] ?>
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita" >
						Papel:
					</td>
					<td>
						<?php $sql = "select
										papdescricao as descricao
									from
										snf.papelcomite
									where
										papstatus = 'A'
									and
										papid = {$membro['papid']}
									order by
										papdescricao" ?>
						<?php echo $db->pegaUm($sql); ?>
					</td>
				</tr>
				<?php $n++; ?>
			<?php endforeach; ?>
		<?php else: ?>
			<tr>
				<td class="SubtituloDireita" >
					CPF:
				</td>
				<td>
					<?php echo campo_texto("memcpf[]","S","S","",18,14,"###.###.###-##","","","",""," id='memcpf_2' onchange='verificaCPFRF(this)' ") ?>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" >
					Nome:
				</td>
				<td>
					<?php echo campo_texto("memnome[]","S","N","",60,255,"","","","",""," id='memnome_2' ") ?>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" >
					Papel:
				</td>
				<td>
					<?php $sql = "select
									papid as codigo,
									papdescricao as descricao
								from
									snf.papelcomite
								where
									papstatus = 'A'
								order by
									papdescricao" ?>
					<?php $db->monta_combo("papid[]",$sql,"S","Selecione...","","","","200","S") ?>
				</td>
			</tr>
		<?php endif; ?>
		<tr>
			<td class="SubtituloTabela center" colspan="2" >
				<?php if(!in_array(PERFIL_CONSULTA,$arrPerfil)): ?>
					<input type="button" name="btn_salvar" onclick="aprovarAdesao()" value="Aprovar" />
					<input type="button" name="btn_salvar_continuar" value="Rejeitar" onclick="rejeitarAdesao()" />
				<?php endif; ?>
				<input type="button" name="btn_voltar" value="Voltar" onclick="history.back(-1)" />
			</td>
		</tr>
	</table>
</form>
<script>
	jQuery("[type='text'],select").attr("disabled","disabled");
	jQuery("[href='javascript:addNovoMembro()'],[src='../imagens/excluir.gif'],[src='../imagens/gif_inclui.gif']").remove();
</script>