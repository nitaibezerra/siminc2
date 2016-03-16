<?php 

function formata_valor_sql($valor){
		
	$valor = str_replace('.', '', $valor);
	$valor = str_replace(',', '.', $valor);
	
	return $valor;
	
}

if($_REQUEST['requisicao'] == 'buscaUniversidade'){
	
	$sql = "SELECT		
				e.entid as codigo,
				CASE WHEN entsig <> '' THEN
					UPPER(entsig) ||  ' - ' || UPPER(entnome)
					ELSE
					UPPER(entnome) END as descricao				
			FROM
				entidade.entidade e 
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid			
			LEFT JOIN
				entidade.endereco ed ON ed.entid = e.entid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = ed.muncod
			WHERE
				e.entstatus = 'A' AND ef.funid  in ('12')
			AND 
				(e.entnome ILIKE '%{$_GET["q"]}%' OR e.entcodent ILIKE '%{$_GET["q"]}%' OR entsig ILIKE '%{$_GET["q"]}%') 
			GROUP BY e.entid, e.entnome , e.entsig, e.entid
			ORDER BY
				 e.entsig, e.entnome";
	
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){		
		echo "{$value['descricao']}|{$value['codigo']}\n";
	}
	
	die;
}

if($_REQUEST['requisicao'] == 'salvarPortaria'){
	
	$ppeid 			= trim($_REQUEST['ppeid']);
	$ppeano 		= $_REQUEST['ppeano'] ? $_REQUEST['ppeano'] : 'null';
	$ppesemestre 	= $_REQUEST['ppesemestre'] ? $_REQUEST['ppesemestre'] : 'null';
	$ppeobs 		= $_REQUEST['ppeobs'];
	$ppenumero		= $_REQUEST['ppenumero'];
	
	if(!$ppeid){
		
		$sql = "insert into academico.portariaprofequival
					(ppeano,ppesemestre,ppeobs, ppenumero)
				values
					({$ppeano}, {$ppesemestre}, '{$ppeobs}', '{$ppenumero}') returning ppeid;";

		$ppeid = $db->pegaUm($sql);
		
	}else{
		
		$sql = "update academico.portariaprofequival set 
					ppeano = {$ppeano},
					ppesemestre = {$ppesemestre},
					ppeobs = '{$ppeobs}',
					ppenumero='{$ppenumero}'
				where ppeid = {$ppeid};";
		
		$db->executar($sql);
	}
	
	if($ppeid){
		
		if(is_array($_REQUEST['entid'])){
			
			$sql = "delete from academico.portariavalor where ppeid = {$ppeid};";
			
			foreach($_REQUEST['entid'] as $k => $entid){
		
				$valor = trim($_REQUEST['valor'][$entid]);
				$valor = $valor ? $valor : 0;
				
//				if($valor){
				
					$valor = formata_valor_sql($valor);
					
					$sql .= "insert into academico.portariavalor
								(entid,ppeid,ptvvalor)
							values
								({$entid}, {$ppeid}, '{$valor}');";
					
//				}
				
				unset($valor);
			}			
			$db->executar($sql);
		}
	}
			
	if($db->commit()){
		$db->sucesso('principal/portariaProfEquivalente', $_REQUEST['ppeid'] ? '&ppeid='.$_REQUEST['ppeid'] : '');
	}
	
	echo "<script>
				alert('Não foi possível salvar a portaria, tente novamente!');
				document.location.href = 'academico.php?modulo=principal/portariaProfEquivalente&acao=A';
		  </script>";
	
}

if($_REQUEST['requisicao'] == 'excluirPortaria'){
	
	$ppeid = $_REQUEST['ppeid'];
	
	if($ppeid){
		$sql = "delete from academico.portariaprofequival where ppeid = {$ppeid}";
		$db->executar($sql);
		if($db->commit()){
			$db->sucesso('principal/portariaProfEquivalente');
		}
	}

	echo "<script>
				alert('Não foi possível excluir a portaria, tente novamente!');
				document.location.href = 'academico.php?modulo=principal/portariaProfEquivalente&acao=A';
		  </script>";
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

$linha1 = 'Portarias Professores Equivalentes';
$linha2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo($linha1, $linha2);	

$habil = 'S';

if($_REQUEST['ppeid']){
	$sql = "select * from academico.portariaprofequival where ppeid = {$_REQUEST['ppeid']}";
	$rs = $db->pegaLinha($sql);
	if($rs) extract($rs);
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<script>
	$(function(){
		$('#btnSalvar').click(function(){
			
//			boSubmete = true;
//			$.each($("[name^='valor']"), function(i,v){
//				valor = trim(v.value);
//				if(valor == ''){					
//					boSubmete = false;
//					$(v).focus();
//					return false;
//				}
//				return false;
//			});
//			
//			if(!boSubmete){
//				alert('O campo valor é obrigatório!');
//				return false;
//			}
			
			if($('[name=ppenumero]').val() == ''){
				alert('O campo Nº da Portaria é obrigatório!');
				$('[name=ppenumero]').focus();
				return false;
			}

			if($('[name=ppeano]').val() == ''){
				alert('O campo Ano é obrigatório!');
				$('[name=ppeano]').focus();
				return false;
			}

			if($('[name=ppesemestre]').val() == ''){
				alert('O campo Semestre é obrigatório!');
				$('[name=ppesemestre]').focus();
				return false;
			}

			$('[name=requisicao]').val('salvarPortaria');
			$('#formulario').submit();
		});

		$('.editarPortaria').click(function(){
			if(this.id){
				document.location.href = 'academico.php?modulo=principal/portariaProfEquivalente&acao=A&ppeid='+this.id;
			}
		});

		$('.excluirPortaria').click(function(){
			if(this.id){
				if(confirm('Deseja excluir a portaria?')){
					$('[name=ppeid]').val(this.id);
					$('[name=requisicao]').val('excluirPortaria');
					$('#formulario').submit();
				}
			}
		});

		$('#btnNovo').click(function(){
			document.location.href = 'academico.php?modulo=principal/portariaProfEquivalente&acao=A';
		});
	});
</script>
<form method="post" name="formulario" id="formulario" id="">
	<input type="hidden" name="ppeid" value="<?php echo $_REQUEST['ppeid']; ?>" />
	<input type="hidden" name="requisicao" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita">Nº da Portaria</td>
			<td>
				<?php echo campo_texto('ppenumero', 'S', $habil, '', 15, 7, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Ano</td>
			<td>
				<?php 
				$arDados = array(
					array('codigo'=>'2010', 'descricao'=>'2010'),
					array('codigo'=>'2011', 'descricao'=>'2011'),
					array('codigo'=>'2012', 'descricao'=>'2012'),
					array('codigo'=>'2013', 'descricao'=>'2013'),
					array('codigo'=>'2014', 'descricao'=>'2014')
				);
				$db->monta_combo('ppeano', $arDados, $habil, 'Selecione...', '', '', '', '', 'S');			
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Semestre</td>
			<td>
				<?php 
				$arDados = array(
					array('codigo'=>'1', 'descricao'=>'1º semestre'),
					array('codigo'=>'2', 'descricao'=>'2º semestre')
				);
				$db->monta_combo('ppesemestre', $arDados, $habil, 'Selecione...', '', '', '', '', 'S');			
				?>	
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro"> Universidades </td>
		</tr>
		<tr>
			<td colspan="2" style="border: 0px solid black;">
			
				<?php 
				
				$stJoin = '';
				$stValue = '';
				if($_REQUEST['ppeid']){
					$stJoin = "LEFT JOIN academico.portariavalor pv ON pv.entid = e.entid AND pv.ppeid = {$_REQUEST['ppeid']}";
					$stValue = "value=\"' || CASE WHEN pv.ptvvalor::varchar IS NULL THEN ' ' WHEN pv.ptvvalor::varchar = '0.00' THEN '0,00' ELSE trim(to_char(pv.ptvvalor,'999G999G999G999D99')) END || '\"";
					$stGroup = "pv.ptvvalor,";
				}
				
				$sql = "SELECT		
							e.entid,
							CASE WHEN entsig <> '' THEN
								UPPER(entsig) ||  ' - ' || UPPER(entnome)
								ELSE
								UPPER(entnome) END as nome,
							upper(mun.mundescricao) as municipio, 
							upper(mun.estuf) as uf,
							'
							<input type=\"hidden\" name=\"entid[]\" value=\"' || e.entid || '\" />
							<input type=\"text\" name=\"valor[' || e.entid || ']\" {$stValue} class=\"normal valoresPortaria\" size=\"10\" onKeyUp=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" />
							' as valor
						FROM
							entidade.entidade e 
						INNER JOIN
							entidade.funcaoentidade ef ON ef.entid = e.entid
						{$stJoin} 
						LEFT JOIN
							entidade.endereco ed ON ed.entid = e.entid
						LEFT JOIN
							territorios.municipio mun ON mun.muncod = ed.muncod
						WHERE
							e.entstatus = 'A' AND ef.funid  in ('12')
						GROUP BY e.entid, e.entnome , e.entsig, mun.mundescricao, mun.estuf, {$stGroup} e.entid
						ORDER BY
							 e.entsig, e.entnome";
				
				$rsUniversidades = $db->carregar($sql);
				?>
				<script>
					$(function(){

						$('[name=buscauni]').autocomplete("academico.php?modulo=principal/portariaProfEquivalente&acao=A&requisicao=buscaUniversidade", {
						  	cacheLength:10,
							width: 440,		
							scrollHeight: 220,
							selectFirst: true,
							autoFill: true	    		
						}).result(function(event, data, formatted) {
							$('[name=valor['+data[1]+']]').focus();
							$('[name=valor['+data[1]+']]').parent().parent().attr('bgcolor', '#fffcc');
							$('[name=buscauni]').val('');
						});
						
					});
				</script>
				Pesquisa:&nbsp;
				<?php echo campo_texto('buscauni', 'S', $habil, '', 45, 7, '', ''); ?>
				<table width="100%" style="padding-right:15px;">
					<thead>
						<tr>
							<th>Nome</th>
							<th width="200">Município</th>
							<th width="60">UF</th>
							<th width=110">Valor</th>
						</tr>
					</thead>
				</table>
				<div style="height:120px;overflow:auto;border: 0px solid black;">
					<table width="100%">
						<tbody>
							<?php $i=0; ?>
							<?php foreach($rsUniversidades as $universidade): ?>			
								<?php if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7'; ?>					
								<tr id="tr<?php echo $universidade['entid']; ?>" bgcolor="<?php echo $marcado; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $marcado; ?>';" >
									<td><?php echo $universidade['nome']; ?></td>
									<td width="200"><?php echo $universidade['municipio']; ?></td>
									<td width="60"><?php echo $universidade['uf']; ?></td>
									<td width="110"><?php echo $universidade['valor']; ?></td>
								</tr>
								<?php $i++; ?>
							<?php endforeach; ?>
						</tbody>
					</table>	
				</div>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Observações</td>
			<td><?php echo campo_textarea('ppeobs', 'N', $habil, '', 65, 8, 300, '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" id="btnSalvar" />
				<?php if($_REQUEST['ppeid']): ?>
					<input type="button" value="Novo" id="btnNovo" />	
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>		

<?php 

$sql = "select			
			'
			<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" id=\"' || ppeid || '\" class=\"editarPortaria\"/>
			<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" id=\"' || ppeid || '\" class=\"excluirPortaria\"/>
			' as acao, 
			ppenumero,
			ppeano,
			ppesemestre,
			ppeobs
		from
			academico.portariaprofequival
		order by
			ppeid desc";

$cabecalho = array('<center>Ações</center>', '<center>Nº Portaria</center>', '<center>Ano</center>', '<center>Semestre</center>', 'Observações');
$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '', '', array(60,80,60,80,null), array('center','center','center','center'));

?>		