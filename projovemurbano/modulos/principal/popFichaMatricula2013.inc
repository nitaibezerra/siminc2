<?php if($_REQUEST['pjuid']) $_SESSION['projovemurbano']['pjuid']=$_REQUEST['pjuid']; 
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/datapicker.pt_Br.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" />
<script>jQuery.noConflict();</script>
<script type="text/javascript" src="./js/projovemurbano.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text'/css" href="../../includes/superTitle.css"/>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function(){
  	jQuery("#caedatanasc").datepicker({
    	maxDate:"+0D", changeMonth: true, changeYear: true, showOtherMonths: true,
    	selectOtherMonths: true, createButton: false, onSelect: function(selectedDate) {
      		jQuery('#caeidade').html(calculaIdade(selectedDate));
    	}
	});
	jQuery('#caeidade').html(calculaIdade(jQuery('#caedatanasc').val()));
});
</script>
<html>
<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<br>
<center>
	<a onclick="print();" style="font-size:14px;color:blue">[Imprimir]</a>
</center>
<br>
<?

if($_REQUEST['caeid']) {
	
	$requisicao = "atualizarEstudantes";
	
	$sql = "SELECT *, caeano||lpad(caeid::varchar,6,'0') as ninscricao FROM projovemurbano.cadastroestudante WHERE caeid='".$_REQUEST['caeid']."'";
	$cadastroestudante = $db->pegaLinha($sql);
	if( is_array($cadastroestudante) ){
		extract($cadastroestudante);
	}
	$caedatanasc = formata_data($caedatanasc);
	$caedataemissaorg = formata_data($caedataemissaorg);
	$caecep = mascaraglobal($caecep,"#####-###");
	$caecpf = mascaraglobal($caecpf,"###.###.###-##");
	
} else {
	$requisicao = "inserirEstudantes";
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita"><b>Status:</b></td>
		<td>
			<?=(($caestatus=="A")?"Ativo":"Inativo") ?> 
		</td>
	</tr>
	<tr id="tr_caejustificativainativacao" <?=(($caestatus!="I")?"style=\"display:none\"":"") ?>>
		<td class="SubTituloDireita"><b>Justificativa para inativação:</b></td>
		<td>
			<?=$caejustificativainativacao?>
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
		<td><?=$caecpf ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td><?=$caenome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de Nascimento:</td>
		<td><?=$caedatanasc ?><input type="hidden" id="caedatanasc" value="<?=$caedatanasc ?>"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Idade:</td>
		<td><div id="caeidade"></div></td>
    		<?php if (isset($caedatanasc) && !empty($caedatanasc)): ?>
      			<script type="text/javascript">
			        jQuery(document).ready(function(){
			          jQuery('#caeidade').val(calculaIdade('<?php echo $caedatanasc; ?>'));
			        });
		      	</script>
		    <?php endif; ?>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sexo:</td>
		<td><?=(($caesexo=="M")?"Masculino": "Feminino") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome da mãe:</td>
		<td>
			<?=$caenomemae	?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome do pai:</td>
		<td><?=$caenomepai ?></td>
	</tr>
		<tr>
		<td class="SubTituloDireita">Naciolidade:</td>
		<td>
		<?=($caenaturalidade=="B"?"Brasileira":"Estrangeira");?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Naturalidade:</td>
			 <td>
      	<? 
						$endestuf = $caeufnaturalidade;
						if($endestuf) : 
							$sql = "SELECT estuf as descricao FROM territorios.estado WHERE estuf = '{$estuf}'";
							$estuf = $db->pegaUm($sql);
							echo $estuf;
						endif; 
				?>
    </td>
  </tr>
  <tr>
    <td id="td_natmuncod"><span style="display:none">Naturalidade</span><span>
     
      </span>
    </td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cor / Raça:</td>
		<td>
			<?
			if( $craid ){
				$sql = "SELECT cradesc as descricao FROM projovemurbano.corraca WHERE craid = ".$craid;
				$craid = $db->pegaUm($sql); 
				echo $craid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Carteira de identidade:</td>
		<td><?=$caenumrg ?> Orgão expedidor: <?=$caeorgaoexpedidorrg ?> Data de emissão: <?=$caedataemissaorg ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Numero de Identificação Social(NIS):</td>
		<td><?=$caenispispasep ?></td>
	</tr>
	<!--<tr>
		<td class="SubTituloDireita">NIT:</td>
		<td><?=$caenit ?></td>
	</tr>-->
	<tr>
		<td class="SubTituloDireita">Pessoa com deficiência:</td>
		<td><?=(($caedeficiencia=="t")?"Sim":"Não") ?> </td>
	</tr>
	<tr id="pessoaDeficiencia_1" <?=(($caedeficiencia=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Deficiência:</b></td>
		<td>
			<? 
			if( $tdeid ){
				$sql = "SELECT tdedesc as descricao FROM projovemurbano.tipodeficiencia WHERE tdeid = ".$tdeid;
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
			if( $tdiid ){
				$sql = "SELECT tdidesc as descricao FROM projovemurbano.transdesininfancia WHERE tdiid = ".$tdiid;
				$tdiid = $db->pegaUm($sql);
				echo $tdiid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Altas Habilidades/Superdotação:</td>
		<td>
			<?=(($caealtashabilidades=="f")?"Sim":"Não") ?>
		</td>
	</tr>
   <tr>
      <td class="SubTituloDireita">Tipo de recurso de acessibilidade que utiliza:</td>
      <td>
        <?php 
          $marcados = $db->carregarColuna('SELECT racid FROM projovemurbano.cadastroestudante_recursosacessibilidade WHERE
          caeid = '.$_REQUEST['caeid']);
        ?>
     <?

     
     foreach ($marcados as $mar) {
				switch ($mar){
					case '1': echo "Braille"; break;
					case '2': echo "Formato Digital (PDF)"; break;
					case '3': echo "Formato Digital Acessível (Mecdaisy, TXT)"; break;
					case '4': echo "Intérprete de Lingua Brasileira de Sinais"; break;
					case '5': echo "Caracteres ampliados"; break;
				}
				echo'<br>';
     }				 
				?>
      </td>
    </tr>
	<tr>
		<td class="SubTituloDireita">Estado Civil:</td>
		<td>
			<? 
			if( $escid ){
				$sql = "SELECT escdesc as descricao FROM projovemurbano.estadocivil WHERE escid = ".$escid;
				$escid = $db->pegaUm($sql);
				echo $escid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tem filhos  ?</td>
		<td>
			<?=(($caefilhos=="t")?"Sim":"Não") ?>
		</td>
	</tr>
	<tr id="temFilhos" <?=(($caefilhos=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita">&nbsp;</td>
		<td colspan="2">
			<?
			if( $caeid ){
				$estqtdfilhos = ",
						(SELECT estqtdfilhos::character(7) 
	                	FROM projovemurbano.estudantefilhos e 
	                	WHERE i.idfid=e.idfid AND e.caeid='".$caeid."'  AND esfstatus = 'A') as estqtdfilhos";
			}
			$sql = "SELECT 
						idfdesc, 
						idfid
						$estqtdfilhos
                	FROM 
                		projovemurbano.idadefilhos i 
                	WHERE 
                		idfstatus='A' 
                	ORDER BY idfid";
			
			$linhas = $db->carregar($sql);
//			ver(simec_htmlentities($sql),d);
			?>
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
						<td valign="top" title="Idade"><?=$linha['idfdesc'] ?></td>
						<td valign="top" title="Quantidade de filhos"><center><?=$linha['estqtdfilhos'] ?></center></td>
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
		<td class="SubTituloDireita">Jovem em cumprimento de medidas sócioeducacionais:</td>
		<td>
			<?=(($caecumpremedidasocioeduc == 't')?"Sim":"Não") ?> 
		</td>
	</tr>
	<tr id="outrosProgramas" <?=(($caebenoutroprog=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Qual ?</b></td>
		<td>
			<?
			if( $pbeid ){ 
				$sql = "SELECT pbedesc as descricao FROM projovemurbano.programabeneficiario WHERE pbeid = ".$pbeid;
				$pbeid = $db->pegaUm($sql);
				echo $pbeid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tem alguma ocupação?</td>
		<td><?=(($caeocupacao=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr id="temOcupacao" <?=(($caeocupacao=="t")?"":"style=\"display:none;\"") ?> >
		<td class="SubTituloDireita"><b>Qual ?</b></td>
		<td>
			<? 
			if( $ocuid ){
				$sql = "SELECT ocudesc as descricao FROM projovemurbano.ocupacao WHERE ocuid = ".$ocuid;
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
			if( $ssoid ){
				$sql = "SELECT ssodesc as descricao FROM projovemurbano.segmentosocial WHERE ssoid = ".$ssoid;
				$ssoid = $db->pegaUm($sql);
				echo $ssoid;
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">É egresso do Programa Brasil Alfabetizado?</td>
		<td><?=(($caebrasilalfa=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">É egresso de outro programa de alfabetização?</td>
		<td><?=(($egressoprogalfabetizacao=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Já participou do projovem urbano?</td>
		<td><?=(($caeparticipouprojovemurbano=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
	<tr>
		<td class="SubTituloDireita">Endereço</td>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" width="20%">CEP</td>
			<td><?=$caecep ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Logradouro</td>
			<td><?=$caelogradouro ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número</td>
			<td><?=$caenumero ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Complemento</td>
			<td><?=$caecomp ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Bairro</td>
			<td><?=$caebairro ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td>
			<? 
			$endestuf = $estuf;
			if($endestuf) : 
				$sql = "SELECT estuf as descricao FROM territorios.estado WHERE estuf = '{$estuf}'";
				$estuf = $db->pegaUm($sql);
				echo $estuf;
			endif; 
			?>

			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Munícipio</td>
			<td>
				<? 
				if($muncod){
					$sql = "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$muncod}'";
					$mundescricao = $db->pegaUm($sql);
					echo $mundescricao;
				}
				?>
			</td>
		</tr>
			<tr>
			<td class="SubTituloDireita">Qual Região de Moradia?</td>
		   <td>
		   <?	if($caetipomoradia == 'r'){ ?>
		  		 	Rural
		  	<? }else{?>	 	
		  			Urbano
		   	<? }?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">E-mail</td>
			<td><?=$caeemail ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone fixo</td>
			<td><?=$caetelfixo ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone celular</td>
			<td><?=$caetelcel ?></td>
		</tr>
		</table>
		
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita">Apresentou histórico escolar ?</td>
		<td><?=(($caehistorico=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Realizou teste de proficiência ?</td>
		<td><?=(($caetestepro=="t")?"Sim":"Não") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Auxília financeiro a receber:</td>
		<td><?=$caeqtddireitobolsa ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Escola</td>
		<td>
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<?
		$sql = "SELECT * FROM projovemurbano.polomunicipio WHERE pjuid='".$pjuid."' AND pmustatus='A'";
		$polomunicipio = $db->pegaLinha($sql);
		
		if($polomunicipio['pmupossuipolo']=="t") :
		?>
		<tr>
			<td class="SubTituloDireita" width="20%">Polo</td>
			<td><? 
			// Adaptação para o perfil Diretor do Pólo
			if( $polid ){
				$sql = "SELECT 'POLO '||polid as descricao 
						FROM projovemurbano.polo 
						WHERE polid = ".$polid;
				$polo = $db->pegaUm($sql);
				echo $polo;
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Núcleo / Escola</td>
			<td id="td_nucleo">
				<? 
				if( $nucid ){
					$sql = "SELECT 
								'NÚCLEO '||nuc.nucid||', SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||COALESCE(', ANEXO: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
							FROM projovemurbano.nucleo nuc
				  			INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				  			INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
				  			INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
				  			INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
				  			WHERE nucid = ".$nucid;
					
					$nucid = $db->pegaUm($sql);
					echo $nucid; 
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Turma</td>
			<td id="td_turma">
				<? 
				$sql = "SELECT 
							turdesc 
						FROM 
							projovemurbano.cadastroestudante cae
						INNER JOIN projovemurbano.turma tur ON tur.turid = cae.turid 
						WHERE caeid = ".$caeid;
				$caeturma = $db->pegaUm($sql);
				echo $caeturma; 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Turno</td>
			<td id="td_turma">
				<? 
				switch ($turno){
					case 'N': echo "Noturno"; break;
					case 'V': echo "Vespertino"; break;
					case 'M': echo "Matutino"; break;
				}				 
				?>
			</td>
		</tr>
		<? endif; ?>
		<? if($polomunicipio['pmupossuipolo']=="f") : ?>
		<tr>
			<td class="SubTituloDireita">Núcleo / Escola</td>
			<td>
				<? 
				if( $nucid ){
					$sql = "SELECT 
								'NÚCLEO '||nuc.nucid||', SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||COALESCE(', ANEXO:'||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
							FROM projovemurbano.nucleo nuc 
						  	INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
						  	INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
						  	WHERE nucid = ".$nucid;
					$nucleo = $db->pegaUm($sql);
					echo $nucleo;
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Turma</td>
			<td id="td_turma">
				<? 
				$sql = "SELECT 
							turdesc 
						FROM 
							projovemurbano.cadastroestudante cae
						INNER JOIN projovemurbano.turma tur ON tur.turid = cae.turid 
						WHERE caeid = ".$caeid;
				$caeturma = $db->pegaUm($sql);
				echo $caeturma; 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Turno</td>
			<td id="td_turma">
				<? 
				switch ($turno){
					case 'N': echo 'Nortuno'; break;
					case 'M': echo 'Matutino'; break;
					case 'V': echo 'Vespertino'; break;					
				} 
				?>
			</td>
		</tr>
		<? endif; ?>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="salvar" value="Salvar" onclick="gravarCadastroEstudantes();"> 
		<? if($caeid) : ?>
		<input type="button" name="novo" value="Novo" onclick="window.location='projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes';">
		<? endif; ?>
		</td>
	</tr>
</table>