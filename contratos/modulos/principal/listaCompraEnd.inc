<?
 
if($_REQUEST['evento']){
	if($_REQUEST['evento'] == "salvar"){
		extract($_REQUEST);
		if(!$coendid){
			$sql = "insert into 
							evento.coenderecoentrega
						(entid,coendcep,coendlog,coendcom,coendbai,comuncod,coestuf,coendnum,coendhrini,coendhrfim,coendrespalmox,coendstatus)
					values
						($entid,'".str_replace("-","",str_replace(".","",$endcep))."','$endlog','$endcom','$endbai','$muncod','$estuf','$endnum','$coendhrini','$coendhrfim','$coendrespalmox','A')";
		}else{
			$sql = "update 
						evento.coenderecoentrega
					set
						entid 			= $entid,
						coendcep 		= '".str_replace("-","",str_replace(".","",$endcep))."',
						coendlog 		= '$endlog',
						coendcom 		= '$endcom',
						coendbai 		= '$endbai',
						comuncod 		= '$muncod',
						coestuf 		= '$estuf',
						coendnum		= '$endnum',
						coendhrini 		= '$coendhrini',
						coendhrfim		='$coendhrfim',
						coendrespalmox	= '$coendrespalmox'
					where
						coendid = $coendid";
		}
	}
	if($_REQUEST['evento'] == "excluir"){
		$sql = "update 
						evento.coenderecoentrega
					set
						coendstatus = 'I' 
					where
						coendid = {$_REQUEST['coendid']}";
	}
	
		
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Operação Realizada com Sucesso!');window.location.href='evento.php?modulo=principal/listaCompraEnd&acao=A&endid=$endid'</script>";
}

require_once APPRAIZ . "adodb/adodb.inc.php";

require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";

require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
?>

<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
   	
   	<script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
</head>
     
<?php

$titulo_modulo = "Compras";
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, 'Dados de Entrega' );

$usucpf = $_SESSION['usucpf'];

$sql = "select 
			pflcod 
	from 
		evento.usuarioresponsabilidade 
	where 
		usucpf = '$usucpf'";

$pflcod = $db->pegaUm($sql);

if($pflcod){
	$sql = "
		select
			ed.endid as endid,
			entEd.entid,
			p.unidsc as descricao,
			ed.endlog as logradouro,
			ed.endcep as cep,
			ed.endbai as bairro,
			ed.estuf as uf,
			ed.endnum as num,
			m.mundsc as municipio
			
		from 
			evento.usuarioresponsabilidade ur
			inner join 
				public.unidade p on ur.unicod = p.unicod
			inner join 
				entidade.entidade e on ur.unicod = e.entunicod
			inner join 
				entidade.entidadeendereco entEd on entEd.entid = e.entid
			inner join 
				entidade.endereco ed on ed.endid = entEd.endid
			left join 
				public.municipio m on m.muncod = ed.muncod
		where
			ur.rpustatus = 'A' and
			ur.usucpf = '$usucpf' and
			ur.pflcod = $pflcod and
			ur.prsano = '".$_SESSION['exercicio']."'";
			
	$RS = @$db->pegaLinha($sql);
}if($RS):
	
	(!$_GET['endid'])? $endid = $RS['endid'] : $endid = $_GET['endid'];
	
	$sql = "select entEntr.*
			from
				evento.coenderecoentrega entEntr
			inner join
				entidade.endereco ende ON entEntr.entid = ende.entid
			inner join
				entidade.entidade ent ON entEntr.entid = ent.entid
			where
				ende.endid = $endid 
			and
				entEntr.coendstatus = 'A'";
	$dados = $db->pegaLinha($sql);
	
	$sql = "select 
				ent.entnome,
				ent.entid
			from
				entidade.endereco ende 
			inner join
				entidade.entidade ent ON ende.entid = ent.entid
			where
				ende.endid = $endid";
	$ent = $db->pegaLinha($sql);
	
	(!$ent['entid'])? $entid = $RS['entid'] : $entid = $ent['entid'];
	
	
?>
	<form name="formulario" id="formulario" action="" method="post" >
	
	<input type="hidden" name="evento" value="salvar" />
	<input type="hidden" name="coendid" value="<?=$dados['coendid'];?>" />
	<input type="hidden" name="entid" value="<?=$entid?>" />
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
				<td style="font-weight: bold" colspan="2">
						Instituição
				</td>
	</tr>
	<tr>
				<td width=25%  class="SubTituloDireita" >
						Nome:
				</td>
				<td id="tdEntdsc">
				<?  !$ent['entnome']? $entdsc = $RS['descricao'] : $entdsc = $ent['entnome']; ?>
				<?= campo_texto( 'entdsc', 'S', 'N', '', 80, 80, '', ''); ?></td>
	</tr>
         <?php
               
	        $endereco = new Endereco($endid);
	        $entidade->enderecos[0] = $endereco;
	
	        
	        echo ''
	                . '<tr>'
	                . '<td style="font-weight: bold" colspan="2">Endereço</td>'
	                . '</tr>'
	
	                . '<tr>'
	                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>'
	                . '<td>'
	                . '<input type="text" name="endcep" onkeyup="this.value=mascaraglobal(\'##.###-###\', this.value);" onblur="Entidade.__getEnderecoPeloCEP(this);" class="CampoEstilo" id="endcep" value="' . (!$dados['coendcep']? $entidade->enderecos[0]->endcep : $dados['coendcep']). '" size="13" maxlength="10" /> <img src="../imagens/obrig.gif" />'
	                . '</td>'
	                . '</tr>'
	
	                . '<tr id="escolha_logradouro_id" style="display:none">'
	                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Selecione o Logradouro:</label></td>'
	                . '<td>'
	                . '<input readonly="readonly" type="text" name="endlog" class="CampoEstilo" id="endlog" value="' . (!$dados['coendlog']? $entidade->enderecos[0]->endlog : $dados['coendlog']) . '" size="48" />'
	                . '</td>'
	                . '</tr>'
	
	                . '<tr>'
	                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Logradouro:</label></td>'
	                . '<td>'
	                . '<input readonly="readonly" type="text" name="endlog" class="CampoEstilo" id="endlogradouro" value="' . (!$dados['coendlog']? $entidade->enderecos[0]->endlog : $dados['coendlog'])  . '" size="48" />'
	                . '</td>'
	                . '</tr>'
	
	                . '<tr>'
	                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>'
	                . '<td>'
	                . '<input type="text" name="endnum" class="CampoEstilo" id="endnum" value="' . (!$dados['coendnum']? $entidade->enderecos[0]->endnum : $dados['coendnum']) . '" size="5" maxlength="8" />'
	                . '</td>'
	                . '</tr>'
	
	                . '<tr>'
	                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>'
	                . '<td>'
	                . '<input type="text" name="endcom" class="CampoEstilo" id="endcom" value="' . (!$dados['coendcom']? $entidade->enderecos[0]->endcom : $dados['coendcom'])  . '" size="48" maxlength="100" />'
	                . '</td>'
	                . '</tr>'
	
	                . '<tr>'
	                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>'
	                . '<td>'
	                . '<input readonly="readonly" type="text" name="endbai" class="CampoEstilo" id="endbai" value="' . (!$dados['coendbai']? $entidade->enderecos[0]->endbai : $dados['coendbai'])  . '" />'
	                . '</td>'
	                . '</tr>'
	
	                . '<tr>'
	                . '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>'
	                . '<td>'
	                . '<input readonly="readonly" type="text" name="mundescricao" class="CampoEstilo" id="mundescricao" value="' . $entidade->enderecos[0]->getMunDescricao() . '" /> '
	                . '<input type="hidden" name="muncod" id="muncod" class="CampoEstilo" value="' . (!$dados['comuncod']? $entidade->enderecos[0]->muncod : $dados['comuncod']) . '" />'
	                . '<input type="hidden" name="endid" id="endid" class="CampoEstilo" value="' .  $endid . '" />'
	                . '<input readonly="readonly" type="text" name="estuf" class="CampoEstilo" id="estuf" value="' . (!$dados['coestuf']? $entidade->enderecos[0]->estuf : $dados['coestuf']) . '" style="width: 5ex; padding-left: 2px" />'
	                . '</td>'
	                . '</tr>'
	                . '<script> document.getElementById(\'endcep\').value = mascaraglobal(\'##.###-###\', document.getElementById(\'endcep\').value);</script>';
	                
                ?>
		<tr>
			<td class="SubTituloDireita" >
					Horário de Funcionamento:
			</td>
			<td><?$coendhrini = $dados['coendhrini']; ?>
				<?= campo_texto( 'coendhrini', 'N', $permissao_formulario, '', 5, 5, '##:##', '','','','','id="coendhrini"'); ?>
				 às 
				 <?$coendhrfim = $dados['coendhrfim']; ?>
				<?= campo_texto( 'coendhrfim', 'N', $permissao_formulario, '', 5, 5, '##:##', '','','','','id="coendhrfim"'); ?>
			</td>
		<tr>
		<tr>
			<td class="SubTituloDireita" >
					Responsável Almoxarifado:
			</td>
			<td><?$coendrespalmox = $dados['coendrespalmox']; ?>
				<?= campo_texto( 'coendrespalmox', 'N', $permissao_formulario, '', 67, 67, '', '','','','','id="coendrespalmox"'); ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" style="text-align: center;"  class="SubTituloDireita" >
					<input type="button" value="Salvar" onclick="salvar();" /> 
					<input type="button" value="Cancelar" onclick="cancelar();" />
			</td>
		</tr>
	</table>
	</form>
	<?php
	$sql = "(select
			distinct
				'<img style=\'cursor:pointer\' align=\"abdmiddle\" title=\'Editar Endereço\' onclick=\'carregarEnderecoEntrega('|| ende.endid ||')\' src=\'../imagens/alterar.gif\' />
				<img style=\'cursor:pointer\' align=\"abdmiddle\" title=\'Excluir Endereço\' onclick=\"alert(\'Operação Não Permitida!\')\" src=\'../imagens/excluir_01.gif\' />',
				ent.entnome as entnome,
				ende.endlog,
				ent.entid,
				mun.mundescricao,
				est.estdescricao
			from
				evento.usuarioresponsabilidade ur
			inner join 
				public.unidade p ON ur.unicod = p.unicod
			inner join 
				entidade.entidade ent ON ur.unicod = ent.entunicod
			inner join 
				entidade.entidadeendereco entEnd ON ent.entid = entEnd.entid
			inner join 
				entidade.endereco ende ON ende.endid = entEnd.endid
			inner join
				territorios.municipio mun on ende.muncod = mun.muncod
			inner join
				territorios.estado est on est.estuf = mun.estuf
			where
				ur.rpustatus = 'A' and
				ur.usucpf = '$usucpf' and
				ur.pflcod = $pflcod and
				ur.prsano = '".$_SESSION['exercicio']."'
			 AND
                            	ent.entid not in ( 	select 
                            							distinct entid 
                            						from 
                            							evento.coenderecoentrega
                            						where 
                            							coendstatus = 'A'))
			
			union
			
			(select
			distinct
				'<img style=\'cursor:pointer\' align=\"abdmiddle\" title=\'Editar Endereço\' onclick=\'carregarEnderecoEntrega('|| ende.endid ||')\' src=\'../imagens/alterar.gif\' />
				<img style=\'cursor:pointer\' align=\"abdmiddle\" title=\'Excluir Endereço\' onclick=\'excluirEnderecoEntrega('|| coend.coendid ||')\' src=\'../imagens/excluir.gif\' />',
				ent.entnome as entnome,
				coend.coendlog,
				ent.entid,
				mun.mundescricao,
				est.estdescricao
			from
				evento.usuarioresponsabilidade ur
			inner join 
				public.unidade p ON ur.unicod = p.unicod
			inner join 
				entidade.entidade ent ON ur.unicod = ent.entunicod
			inner join 
				entidade.entidadeendereco entEnd ON ent.entid = entEnd.entid
			inner join 
				entidade.endereco ende ON ende.endid = entEnd.endid
			inner join
				evento.coenderecoentrega coend ON coend.entid = ent.entid
			inner join
				territorios.municipio mun on coend.comuncod = mun.muncod
			inner join
				territorios.estado est on est.estuf = mun.estuf
			where
				ur.rpustatus = 'A' and
				ur.usucpf = '$usucpf' and
				ur.pflcod = $pflcod and
				ur.prsano = '".$_SESSION['exercicio']."' and
				coend.coendstatus = 'A')";
	
	$sql2 = "(select 
								distinct
								'<img style=\'cursor:pointer\' title=\'Editar Endereço\' onclick=\'carregarEnderecoEntrega('|| ende.endid ||')\' src=\'../imagens/alterar.gif\' />
								<img style=\'cursor:pointer\' title=\'Excluir Endereço\' onclick=\"alert(\'Operação Não Permitida!\')\" src=\'../imagens/excluir_01.gif\' />',
								et.entnome as entnome,
								ende.endlog,
								mun.mundescricao
							FROM
								entidade.entidade et
							inner join 
								entidade.entidadeendereco entEnd ON et.entid = entEnd.entid
							inner join 
								entidade.endereco ende ON ende.endid = entEnd.endid
							inner join
								territorios.municipio mun on ende.muncod = mun.muncod
							inner join
								territorios.estado est on est.estuf = mun.estuf
							INNER JOIN
                            	entidade.funcaoentidade ef ON et.entid = ef.entid
							INNER JOIN
                            	entidade.funentassoc ea ON ea.fueid = ef.fueid
                            WHERE
                            	ea.entid = |agrupador|
                            AND
                            	et.entid not in ( 	select 
                            							distinct entid 
                            						from 
                            							evento.coenderecoentrega
                            						where 
                            							coendstatus = 'A'))
                            union
                            
                            (select 
								distinct
								'<img style=\'cursor:pointer\' title=\'Editar Endereço\' onclick=\'carregarEnderecoEntrega('|| ende.endid ||')\' src=\'../imagens/alterar.gif\' />
								<img style=\'cursor:pointer\' title=\'Excluir Endereço\' onclick=\'excluirEnderecoEntrega('|| coend.coendid ||')\' src=\'../imagens/excluir.gif\' />',
								et.entnome as entnome,
								coend.coendlog,
								mun.mundescricao
							FROM
								entidade.entidade et
							inner join 
								entidade.entidadeendereco entEnd ON et.entid = entEnd.entid
							inner join 
								entidade.endereco ende ON ende.endid = entEnd.endid
							INNER JOIN
                            	entidade.funcaoentidade ef ON et.entid = ef.entid
							INNER JOIN
                            	entidade.funentassoc ea ON ea.fueid = ef.fueid
                            INNER JOIN
                            	evento.coenderecoentrega coend ON coend.entid = et.entid 
                            inner join
								territorios.municipio mun on coend.comuncod = mun.muncod
							inner join
								territorios.estado est on est.estuf = mun.estuf
                            WHERE
                            	ea.entid = |agrupador|
                             and
                            	coend.coendstatus = 'A')";
	
	$sqlAgrupador = array("sql" => $sql2, "cabecalho"=> array("Ações","Unidade","Endereço","Município"), "agrupador" => "entid", "link" => array(), "campo" => array(), "get" => array(), "arrOff" => array(), "exibeLink" => array());
	
	
	$cabecalho = array("Ações","Unidade","Endereço","Município","Estado");
	
	//listaUnidadesLink('tabela_unidades',$sql,'',$cabecalho,$sqlAgrupador,"N");
	?>
<?endif; 

if(!$RS):
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align="center" >
					Instituição não atribuída. Favor atribuir uma Instituição ao perfil.
			</td>
		</tr>
	</table>
<? endif; ?>
<script>

var d = document;

function salvar(){
	if(!d.getElementById('coendhrini').value){
		alert('Informe o Horário de Funcionamento.');
		d.getElementById('coendhrini').focus();
		return false;
	}
	if(!d.getElementById('coendhrfim').value){
		alert('Informe o Horário de Funcionamento.');
		d.getElementById('coendhrfim').focus();
		return false;
	}
	if(!d.getElementById('coendrespalmox').value){
		alert('Informe o Responsável Almoxarifado.');
		d.getElementById('coendrespalmox').focus();
		return false;
	}
	
	d.formulario.submit();
}


function cancelar(){
	window.location.href='evento.php?modulo=principal/listaCompraEnd&acao=A';
}


function carregarEnderecoEntrega(endid){
	window.location.href='evento.php?modulo=principal/listaCompraEnd&acao=A&endid=' + endid;
}

function excluirEnderecoEntrega(coendid){
	if(confirm('Deseja realmente excluir?')){
		window.location.href='evento.php?modulo=principal/listaCompraEnd&acao=A&evento=excluir&coendid=' + coendid;
	}
}

</script>