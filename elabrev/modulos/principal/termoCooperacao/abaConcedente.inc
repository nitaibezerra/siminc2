<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
</script>
<?php

verificaCodigoTermoSessao();

if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

if ($_SESSION['elabrev']['tcpid']) {
			
	$sql = "SELECT	
				ungcodconcedente,
				ungcodproponente,
				unridconcedente,
				CASE WHEN ungcodpoliticafnde IS NOT NULL THEN ungcodpoliticafnde || '_ungcod'
				ELSE dircodpoliticafnde || '_dircod' END AS codpoliticafnde
			FROM 
				monitora.termocooperacao
			WHERE 
				tcpid = ".$_SESSION['elabrev']['tcpid'];
	
	$dadosTermo = $db->pegaLinha($sql);
}

$dados = recuperaDadosUG( $dadosTermo['ungcodconcedente'] );

if($dados){
	
	$ungcod = $dados['ungcod'];
	
	$sqlB = "select	
				gun.ungcodresponsavel
			FROM public.unidadegestora ung
			inner join public.unidade uni 		on ung.unicod = uni.unicod
			inner join public.grupounidade gun 	on gun.gunid = uni.gunid
			WHERE ung.ungcod = '{$dados['ungcod']}'";
	
	$registro = $db->pegaUm($sqlB);
}

$repConc = buscarRepreConcedente($ungcod);

$habilita_pro_con = 'N';
?>

<form method="post" name="formulario" id="formulario">
	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="requisicao" name="requisicao" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	
	<input type="hidden" id="abaAtual" name="abaAtual" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom">
				<center><b>Unidade Gestora <?=($aba =='concedente' ? 'Concedente' : 'Proponente')?></b></center>
			</td>
		</tr>
	</table>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Código Unidade Gestora</td>
			<td>
			<?php 
				$where = "";				
				if($registro != ''){
					$where = "AND ungcod = '{$registro}'"; 
				}
				$sql = "SELECT	
							ungcod as codigo,
							ungabrev||' / '||ungdsc as descricao
						FROM public.unidadegestora
						WHERE ungstatus = 'A' 
						".$where."
						AND ungcod in (
						'150001','150003','150004','150019','150016','150011',
						'150028','152390','152389','153173','154003','153978',
						'155007','150014','150002'
						)
						ORDER BY 2";
				
				$db->monta_combo('ungcod',$sql, $habilita_Und_G, $registro != '' ? '' :  'Selecione...','atualizaUG',$opc,'','445','S', 'ungcod', '', $dadosTermo['ungcodconcedente'], $title= null);
			?>
			</td>
		</tr>
		<tr id="tr_ungpolitica" style="display: <?if($ungcod != '153173'){echo 'none';}?>">
			<td class="SubTituloDireita" width="15%">Responsável pela Política</td>
			<td>
			<?php 

				$sql = "SELECT 
							dircod || '_dircod' as codigo, 
							''||ug.ungabrev||' / ' || dirdsc as descricao
						FROM public.unidadegestora ug
							inner join elabrev.diretoria d ON d.ungcod = ug.ungcod
						WHERE ungstatus='A' and dirstatus = 'A'
						AND d.dircod IN (38,39,41,42,43,58)
				
						UNION ALL
						
						SELECT	
							ungcod || '_ungcod' as codigo,
							ungabrev||' / '||ungdsc as descricao
						FROM public.unidadegestora
						WHERE ungstatus = 'A' 
						AND ungcod in ('150019','150028','150016')
				";
				
				$db->monta_combo('codpoliticafnde',$sql, 'S', 'Selecione...','',$opc,'','445','S', 'codpoliticafnde', '', $dadosTermo['codpoliticafnde'], $title= null);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Código da Gestão</td>
			<td><div id="divgescod"><?= $dados['gescod'] ?></div></td>
		</tr>
        <tr>
            <td class="SubTituloDireita" width="15%">CNPJ</td>
            <td><div id="divungcnpj"><?= formatar_cnpj($dados['ungcnpj']); ?></div></td>
        </tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Endereço</td>
			<td><?=campo_texto('ungendereco','S',$habilita_pro_con,'',70,100,'','', '', '', '', 'id="ungendereco"', '', $dados['ungendereco']);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Bairro</td>
			<td><?=campo_texto('ungbairro','S',$habilita_pro_con,'',70,100,'','', '', '', '', 'id="ungbairro"', '', $dados['ungbairro']);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">UF</td>
			<td>
			<?php 
			$sql = "SELECT
						estuf as codigo,
						estdescricao as descricao
					FROM
						territorios.estado
					ORDER BY
						2";
			$db->monta_combo('estuf',$sql, $habilita_pro_con,'Selecione...','atualizaComboMunicipio','','',200,'S', 'estuf', '', $dados['estuf'], '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Município</td>
			<td id="td_muncod">
			<?php 
			if( $dados['estuf'] != '' ){
				$sql = "
					SELECT 	muncod as codigo,
							mundescricao as descricao
					FROM territorios.municipio
					WHERE estuf ilike '".$dados['estuf']."'
					ORDER BY 2
				";
			}else{
				$sql = "SELECT
							'' as codigo,
							'Selecione uma UF' as descricao";
			}
			$db->monta_combo('muncod',$sql, $habilita_pro_con,'Selecione...','','','',200,'S', 'estuf', '', $dados['muncod'], '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">CEP</td>
			<td >
				<?=campo_texto('ungcep','S',$habilita_pro_con,'',15,10,'##.###-###','', '', '', '', 'id="ungcep"', '', $dados['ungcep']);?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Telefone</td>
			<td >
				<?=campo_texto('ungfone','S',$habilita_pro_con,'',15,14,'##-####-####','', '', '', '', 'id="ungfone"', '', $dados['ungfone']);?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">E-mail</td>
			<td >
				<?=campo_texto('ungemail','S',$habilita_pro_con,'',70,100,'','', '', '', '', 'id="ungemail"', '', $dados['ungemail']);?>
			</td>
		</tr>
	</table>

    <?php
    $strSQL = "
        select
            u.usunome, wh.usucpf
        from workflow.historicodocumento wh
        inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
        where
            wh.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
            and wh.aedid in (1605, 1610)
        order by wh.hstid desc limit 1
    ";

    $responsavel = $db->pegaLinha($strSQL);

    if (is_array($responsavel)) :
        ?>
        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="2" class="SubTituloCentro" style="background-color: #CFCFCF;">Coordenação Responsável</td>
            </tr>
            <tr>
                <td class="SubTituloCentro">Nome</td>
                <td class="SubTituloCentro">CPF</td>
            </tr>
            <tr>
                <td align="center"><?= $responsavel['usunome']; ?></td>
                <td align="center"><?= formatar_cpf($responsavel['usucpf']); ?></td>
            </tr>
        </table>
    <?php endif; ?>

	<?php if( $repConc['usucpfconcedente'] != ''): ?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTituloCentro" style="background-color: #CFCFCF;">Representante Legal</td>
			</tr>
		</table>
		<table id="responsaveis" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">	
			<?php
				litaResponsavelUgConc($ungcod);
			?>
		</table>
	<?php elseif( $repConc['usucpfconcedente'] == ''): ?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTituloCentro" style="background-color: #CFCFCF;">Representante Legal</td>
			</tr>
		</table>	
		<table id="responsaveis" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
			<tr>
				<td class="SubTituloCentro" style="color:red;">Selecione um Representante Legal definindo uma unidade gestora.</td>
			</tr>
		</table>		
	<?php endif; ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
				<?php if($habilitaSalvar == 'S'): ?>
					<input type="button" id="proponente" value="Gravar e Voltar" class="navegarSalvar"/> 
					<input type="button" id="salva" value="Gravar" class="salvar"/>
					<input type="button" id="descentralizacao" value="Gravar e Continuar" class="navegarSalvar"/>
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>