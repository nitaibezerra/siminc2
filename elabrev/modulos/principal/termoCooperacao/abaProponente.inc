<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
</script>
<?php

if (!isset($_GET['ungcod']) || empty($_GET['ungcod'])) {
    verificaCodigoTermoSessao();
}

if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

if($_SESSION['elabrev']['tcpid']){	
			
	$sql = "SELECT	
				ungcodconcedente,
				ungcodproponente,
				unridconcedente,
				ungcodpoliticafnde,
				dircodpoliticafnde
			FROM
				monitora.termocooperacao
			WHERE 
				tcpid = ".$_SESSION['elabrev']['tcpid'];

	$dadosTermo = $db->pegaLinha($sql);
}

$ungproponente = $dadosTermo['ungcodproponente'] ? $dadosTermo['ungcodproponente'] : $_GET['ungcod'];
//ver($ungproponente); exit;

if ($ungproponente) {
    $dados = recuperaDadosUG($ungproponente);
}

$ungcod = $dados['ungcod'] ? $dados['ungcod'] : null;
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
			<td><?=$dados['ungcod'] ?><input type="hidden" name="ungcod" value="<?=$dados['ungcod'] ?>"/></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Razão Social</td>
			<td><?=$dados['ungdsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Código da Gestão</td>
			<td><div id="divgescod"><?=$dados['gescod'] ?></div></td>
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
    if ($_SESSION['elabrev']['tcpid']) {
        $strSQL = "
            select
                u.usunome, wh.usucpf
            from workflow.historicodocumento wh
            inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
            where
                wh.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
                and wh.aedid = 1595
        ";

        $responsavel = $db->pegaLinha($strSQL);
    }

    if (is_array($responsavel)) :
        ?>
        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="2" class="SubTituloCentro" style="background-color: #CFCFCF;">Área Técnica Responsável</td>
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

	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" style="background-color: #CFCFCF;">Representante Legal</td>
		</tr>
	</table>
	<table id="responsaveis" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">	
		<?php
		if( $ungcod == '' ){
			if(!empty($_SESSION['elabrev']['unicod'])){	
				$sql = "SELECT
							ungcod
						FROM
							public.unidadegestora u
						WHERE 
							".(is_array($_SESSION['elabrev']['unicod']) ? "u.unicod IN ('".implode("','", $_SESSION['elabrev']['unicod'])."')" : "u.unicod = '".$_SESSION['elabrev']['unicod']."'");
				
				$ungcod = $db->carregarColuna($sql);
			}
		}	
		?>		
		<?php if(empty($ungcod)): ?>
	
			<tr align="center">
				<td align="center">
					<strong style="color:#cc0000;">Não há Unidade Gestora associada.</strong>
				</td>
			</tr>
			
		<?php else: ?>
		
			<?php $resp = litaResponsavelUgProp($ungcod); ?>
					
			<?php if( $resp['usunome'] ): ?>
				<tr>
					<td id="tr_usucpf" value="1" class="subtitulocentro" width="15%">CPF</td>
					<td class="subtitulocentro" width="15%">Nome</td>
					<td class="subtitulocentro" width="55%">Função</td>
					<td class="subtitulocentro" width="55%">E-mail</td>
					<td class="subtitulocentro" width="55%">Status</td>
				</tr>			
				<tr>
					<td align="center">
						<input type="hidden" id="usucpf" name="usucpf" value="<?=$resp['usucpf'] ?>" />
						<input type="hidden" id="entid" name="entid" value="<?=$resp['entid'] ?>" /> 
						<? echo formatar_cpf($resp['usucpf']); ?> 
					</td>
					<td align="center"> <?=$resp['usunome'] ?> </td>
		  			<td align="center"> Representante Legal </td> 
					<td align="center"> <?=$resp['usuemail'] ?> </td>										
					<td align="center"> <?=$resp['rpustatus'] ?> </td>
				</tr>
			<?php else: ?>
			
				<?php $habilitaSalvar = 'N'; ?>
				
				<tr>
					<td class="SubTituloCentro" style="background-color: #CFCFCF;">Representante Legal</td>
				</tr>
				<tr>
					<td class="SubTituloCentro" style="color:red;">
						Não há Representante Legal vinculado. <br> É necessário que haja um representante vinculado para dar continuidade ao cadastramento do termo.
					</td>
				</tr>
			<?php endif; ?>
			 
		<?php endif; ?>
	</table>	
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
				<?php if($habilitaSalvar == 'S'): ?> 
					<input type="button" id="salva" value="Gravar" class="salvar"/>					
					<input type="button" id="concedente" value="Gravar e Continuar" class="navegarSalvar"/>					
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>