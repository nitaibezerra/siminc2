<?php if($_REQUEST['pjuid']) $_SESSION['projovemcampo']['pjuid']=$_REQUEST['pjuid']; ?>
<html>
<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<?

if($_REQUEST['estid']) {
	$requisicao = "atualizarEstudantes";
	
	$sql = "SELECT *, '2014'||lpad(estid::varchar,6,'0') as ninscricao FROM projovemcampo.estudante WHERE estid='".$_REQUEST['estid']."'";
	$estudante = $db->pegaLinha($sql);
// 	ver($estudante);
	if( is_array($estudante) ){
		extract($estudante);
	}
	$estdatanascimento = formata_data($estdatanascimento);
	$estdataemissaorg = formata_data($estdataemissaorg);
	$estcep = mascaraglobal($estcep,"#####-###");
	$estcpf = mascaraglobal($estcpf,"###.###.###-##");
} else {
	$requisicao = "inserirEstudantes";
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="./js/projovemcampo.js"></script>
<script language="javascript" type="text/javascript">
	jQuery(document).ready(function(){
		jQuery('#estidade').val(calculaIdade('<?php echo $estdatanascimento; ?>'));
	});
</script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<br>
<center>
	<a onclick="print();" style="font-size:14px;color:blue">[Imprimir]</a>
</center>
<br>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita"><b>Status:</b></td>
		<td>
			<?=(($eststatus=="A")?"Ativo":"Inativo") ?> 
		</td>
	</tr>
	<tr id="tr_estjustificativainativacao" <?=(($eststatus!="I")?"style=\"display:none\"":"") ?>>
		<td class="SubTituloDireita"><b>Justificativa para inativação:</b></td>
		<td>
			<?=$estmotivoinativacao?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>N° de matricula:</b></td>
		<td>
			<?=$ninscricao?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">CPF:</td>
		<td><?=$estcpf ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td><?=$estnome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de Nascimento:</td>
		<td><?=$estdatanascimento ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Idade:</td>
		<td><?=campo_texto('estidade', 'N', 'N', 'Idade', 5, 2, '###', '', '', '', 0, 'id="estidade" required disabled'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sexo:</td>
		<td><?=(($estsexo=="M")?"Masculino": "Feminino") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome da mãe:</td>
		<td>
			<?=$estnomemae	?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome do pai:</td>
		<td><?=$estnomepai ?></td>
	</tr>
		<tr>
		<td class="SubTituloDireita">Naciolidade:</td>
		<td>
		<?=($estnaturalidade=="B"?"Brasileira":"Estrangeira");?>
		</td>
	</tr>
	<tr>
		<tr>
			<td class="SubTituloDireita" rowspan ="2">Naturalidade:</td>
			<td>
					<?=($estestufnasc)?>
			</td>
		</tr>
		<tr>
			<td>
				<?$sql = "SELECT
								mundescricao
							FROM
								territorios.municipio
						  WHERE
								muncod = '$estmuncodnasc' ";
				  $municipio = $db->pegaUm($sql);
				  echo $municipio;?>
			</td>
		</tr>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cor / Raça:</td>
		<td>
			<?
			if( $corid ){
				$sql = "SELECT cordescricao as descricao FROM projovemcampo.corraca WHERE corid = ".$corid;
				$craid = $db->pegaUm($sql); 
				echo $craid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Carteira de identidade:</td>
		<td><?=$estnumrg ?> Orgão expedidor: <?=$estorgaoexpedidorrg ?> Data de emissão: <?=$estdataemissaorg ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">NIS / PIS / PASEP / NIT:</td>
		<td><?=$estnis ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Pessoa com deficiência:</td>
		<td><?=(($estdeficiencia=="t")?"Sim":"Não") ?> </td>
	</tr>
	<tr id="pessoaDeficiencia_1" <?=(($estdeficiencia=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Deficiência:</b></td>
		<td>
			<? 
			if( $tdeid ){
				$sql = "SELECT tdedesc as descricao FROM projovemcampo.tipodeficiencia WHERE tdeid = ".$tdeid;
				$tdeid = $db->pegaUm($sql);
				echo $tdeid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Transtorno global do desenvolvimento:</td>
		<td>
			<? 
			if( $trdid ){
				$sql = "SELECT trddescricao as descricao FROM projovemcampo.transtornodesenvolvimento WHERE trdid = $trdid";
				$tdiid = $db->pegaUm($sql);
				echo $tdiid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Altas Habilidades/Superdotação:</td>
		<td>
			<?=(($estaltashabilidades=="t")?"Sim":"Não") ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de recurso de acessibilidade que utiliza:</td>
		<td>
			<?php 
			$sql = "SELECT
						tradescricao as descricao
					FROM
						projovemcampo.recursoacessibilidade r
					INNER JOIN projovemcampo.estudanterecursoacessibilidade c ON c.traid = r.traid 
					WHERE
						estid = ".$_REQUEST['estid'];
			$recursos = $db->carregarColuna($sql);
			if( is_array($recursos) && count($recursos) > 0 ){
				foreach($recursos as $recurso){
					echo "-$recurso<br>";
				}
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Estado Civil:</td>
		<td>
			<? 
			if( $escid ){
				$sql = "SELECT escdescricao as descricao FROM projovemcampo.estadocivil WHERE escid = {$escid}";
				$escid = $db->pegaUm($sql);
				echo $escid;
			}
			?>
		</td>
	</tr>
	<tr>
	    <td class="SubTituloDireita">Tem filhos ?</td>
	    <td><?=(($estfilhos=="t")?"Sim":"Não") ?> </td>
    </tr>
    <tr id="temFilhos" <?=(($estfilhos == 't')?'':'style="display:none"')?>>
    	<td class="SubTituloDireita"><b>Quantos filhos?</b></td>
    	<?if( $estfilhos=="t" ){
				$estqtdfilhos = ",
						(SELECT qtdfilhos::character(7) 
	                	FROM projovemcampo.estudantefaixaetariafilhos e 
	                	WHERE i.fefid=e.fefid AND e.estid='".$estid."'  --AND esfstatus = 'A'
						) as qtdfilhos";
				$sql = "SELECT 
							fefdescricao, 
							fefid
							$estqtdfilhos
	                	FROM 
	                		projovemcampo.faixaetariafilhos i 
	                	WHERE 
	                		fefstatus='A' 
	                	ORDER BY fefid";
				$linhas = $db->carregar($sql);
    	}
			?>
    	<td colspan="2">
				<table width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color:333333;">
					<thead>
						<tr>
							<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Idade</td>
							<td valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">Quantidade de filhos</td>
						</tr> 
					</thead>
					<tbody>
				<? 
				$arr = Array();
				for( $x=0; $x < 10; $x++ ){
					$arr[] = Array( 'codigo' => $x, 'descricao' => $x );
				}
				if( is_array($linhas) ){
					foreach($linhas as $linha){
				?>
						<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
							<td valign="top" title="Idade"><?=$linha['fefdescricao'] ?></td>
							<td valign="top" title="Quantidade de filhos"><?=$db->monta_combo("estqtdfilhos[{$linha['fefid']}]", $arr, 'N', 'Selecione', '', '', '', '', 'S', "estqtdfilhos[{$linha['fefid']}]", null, $linha['qtdfilhos'], null, 'required'); ?></td>
						</tr>
				<? 
					}
				}
			?>
				</tbody>
			</table>
    	</td>
    </tr>
	<tr>
		<td class="SubTituloDireita">Beneficiários de outros programas:</td>
		<td>
			<?=(($estbenoutroprog=="t")?"Sim":"Não") ?> 
		</td>
	</tr>
	<tr id="outrosProgramas" <?=(($estbenoutroprog=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Qual ?</b></td>
		<td>
			<?
			if( $pbeid ){ 
				$sql = "SELECT pbedesc as descricao FROM projovemcampo.programabeneficiario WHERE pbeid = ".$pbeid;
				$pbeid = $db->pegaUm($sql);
				echo $pbeid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tem alguma ocupação?</td>
		<td><?=(($estocupacao=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr id="temOcupacao" <?=(($estocupacao=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Qual ?</b></td>
		<td>
			<? 
			if( $ocuid ){
				$sql = "SELECT ocudescricao as descricao FROM projovemcampo.ocupacao WHERE ocuid = {$ocuid}";
				$ocuid = $db->pegaUm($sql);
				echo $ocuid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Segmento Social:</td>
		<td>
			<? 
			if( $sesid ){
				$sql = "SELECT sesdescricao as descricao FROM projovemcampo.segmentosocial WHERE sesid = {$sesid}";
				$sesid = $db->pegaUm($sql);
				echo $sesid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">É egresso do Programa Brasil Alfabetizado?</td>
		<td><?=(($estbrasilalfa=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">É egresso de outro programa de alfabetização?</td>
		<td><?=(($egressoprogalfabetizacao=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Endereço</td>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" width="20%">CEP</td>
			<td><?=$estendcep ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Logradouro</td>
			<td><?=$estendlogradouro ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número</td>
			<td><?=$estendnumero ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Complemento</td>
			<td><?=$estendcomplemento ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Bairro</td>
			<td><?=$estendbairro ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td>
			<?=$estendestuf	?>

			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Munícipio</td>
			<td>
				<? 
				if($estendmuncod){
					$sql = "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$estendmuncod}'";
					$mundescricao = $db->pegaUm($sql);
					echo $mundescricao;
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">E-mail</td>
			<td><?=$estemail ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone fixo</td>
			<td><?=$esttelefone ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone celular</td>
			<td><?=$estcelular ?></td>
		</tr>
		</table>
		
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita">Apresentou histórico escolar ?</td>
		<td><?=(($esthistoricoescolar=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Realizou teste de proficiência ?</td>
		<td><?=(($esttesteproficiencia=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Auxília financeiro a receber:</td>
		<td><?=$estauxiliosareceber ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Escola</td>
		<td>
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita">Escola</td>
				<td>
					<? 
					if( $turid ){
						$sql = "SELECT 
									entnome as descricao 
								FROM entidade.entidade ent
								INNER JOIN projovemcampo.turma t ON t.entid = ent.entid
								WHERE 
									turid = {$turid}";
						$escola = $db->pegaUm($sql);
						echo $escola;
					}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Turma</td>
				<td id="td_turma">
					<? 
					$sql = "SELECT 
								turdescricao 
							FROM 
								 projovemcampo.turma 
							WHERE turid = {$turid}";
					$estturma = $db->pegaUm($sql);
					echo $estturma; 
					?>
				</td>
			</tr>
<!-- 			<tr> -->
<!-- 				<td class="SubTituloDireita">Turno</td> -->
<!-- 				<td id="td_turma"> -->
					<? 
// 					switch ($turno){
// 						case 'N': echo 'Nortuno'; break;
// 						case 'M': echo 'Matutino'; break;
// 						case 'V': echo 'Vespertino'; break;					
// 					} 
// 					?>
<!-- 				</td> -->
<!-- 			</tr> -->
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="salvar" value="Salvar" onclick="gravarCadastroEstudantes();"> 
		<? if($estid) : ?>
		<input type="button" name="novo" value="Novo" onclick="window.location='projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes';">
		<? endif; ?>
		</td>
	</tr>
</table>