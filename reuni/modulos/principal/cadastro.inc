<?php

@session_start();

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/arquivo.inc';
include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'reuni/www/funcoes.php';
print '<br/>';
//$db->cria_aba( $abacod_tela, $url, '' );
$db->cria_aba( $abacod_tela, $url, "&unicod=" . $_REQUEST['unicod'] );
monta_titulo( $titulo_modulo, '' );

$_SESSION['unicod'] = $_REQUEST['unicod'];


$sql = "
		select
			codPai,
			dscPai,
			ordemPai,
			codFilho,
			dscFilho,
			ordemFilho,
			codPergunta,
			dscPergunta,
			ordemPergunta,
			obrPergunta,
			qtdCaracteres
		from
			(
				select
					gp.grpcod as codPai,
					gp.grpdsc as dscPai,
					gp.grpord as ordemPai,
					gp2.grpcod as codFilho,
					gp2.grpdsc as dscFilho,
					gp2.grpord as ordemFilho,
					p.prgcod as codPergunta,
					p.prgdsc as dscPergunta,
					p.prgord as ordemPergunta,
					p.prgobr as obrPergunta,
					p.prgqtdcar as qtdCaracteres
				from reuni.grupopergunta gp
					inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
					inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
			union all
				select
					gp.grpcod as codPai,
					gp.grpdsc as dscPai,
					gp.grpord as ordemPai,
					null as codFilho,
					null as dscFilho,
					null as ordemFilho,
					p.prgcod as codPergunta,
					p.prgdsc as dscPergunta,
					p.prgord as ordemPergunta,
					p.prgobr as obrPergunta,
					p.prgqtdcar as qtdCaracteres
				from reuni.grupopergunta gp
					inner join reuni.pergunta p ON p.grpcod = gp.grpcod
				where gp.grpcodfil is null
			) as foo
		order by
			ordemPai,
			ordemFilho,
			ordemPergunta
";

$result = $db->carregar($sql);


function montaSituacao($nome='',$readOnly=false,$codParecer=''){
	global  $db;
	$sql = "select sitdsc , sitcod from reuni.situacao";
	$dado = $db->carregar($sql);
	$saida = '';
	$check = '';

	if($readOnly == false) {
		$sql = "select sitdsc from reuni.situacao where  sitcod = $codParecer";
		return $db->pegaUm($sql);

	}

	foreach ($dado as $row)
	{

		if($row['sitcod'] == $codParecer) $check = "checked='checked'";
		$saida .= "<input type='radio' name='situacao_$nome' $read $check value='".$row['sitcod']."'>". $row['sitdsc']	. "&nbsp;";
		$check = '';
	}
	return $saida;
}



function espaco($qtd){
	$i=0;
	while($i<$qtd){
		$saida .= '&nbsp;';
		$i++;
	}
	return $saida;
}

function paiPreenchidoQuestao($unicod,$codPai){
	global  $db;
	$docid = (integer)$db->pegaUm( "select docid from reuni.unidadeproposta where unicod = '".$unicod."'" );
	$fase = wf_pegarEstadoAtual($docid);

	if($fase['esdid'] == REUNI_ESTADO_IFES or $fase['esdid'] == REUNI_ESTADO_ELABORACAO){
		$sql = "select count(a.grpcod)- count(r.prgcod) as qtd from reuni.grupopergunta gp
				inner join reuni.pergunta a ON a.grpcod = gp.grpcod left join 
				(Select p.prgcod, r.unicod, r.rspdsc from reuni.pergunta p inner join reuni.resposta r ON r.prgcod = p.prgcod where r.unicod = '$unicod' and char_length(r.rspdsc) > 0  
				group by p.prgcod, r.unicod, r.rspdsc) as r ON r.prgcod = a.prgcod where gp.grpcod = $codPai and gp.prjcod = 1 group by gp.grpcod";

		$dado = $db->pegaUm($sql);

		if($dado == 0)
		return '<img src="/imagens/check_p.gif">';
	}
	return false;
}

/*
 function encaminhar($unicod){

 global $db;

 if(permissaoReuni(array(IFES_APROVACAO),$unicod)){
 $sql1 = "select count(*) as qtd from reuni.pergunta p inner join reuni.resposta r on p.prgcod = r.prgcod where char_length(r.rspdsc) > 0 and r.unicod = $unicod and p.prgobr = 't'";
 $sql2 = "select count(*) as qtdPerguntas from reuni.pergunta where prgobr = 't'";

 $qtdRespostaUnicod = $db->pegaUm($sql1);
 $qtdResposta = $db->pegaUm($sql2);
 $status = status($unicod);
 if($qtdRespostaUnicod == $qtdResposta && dadoUnidadePreenchido($unicod) && ($status == 'Não iniciado' or $status == 'Em elaboração' )){
 return '<a href="reuni.php?modulo=principal/cadastro&acao=C&unicod='.$unicod.'&enviar=send"><img src="/imagens/send.png" border="0" alt="Enviar"  onclick="historicoTramitacoes()" onmouseover="window.SuperTitleOn( this , \'O formulário está 100% preenchido, clique aqui para envia-lo para aprovação.\' )"></a><p>';
 }else {
 return false;
 }

 }
 else
 return false;
 }
 */
function paiPreenchido($unicod,$codPai){
	global $db;

	$docid = (integer)$db->pegaUm( "select docid from reuni.unidadeproposta where unicod = '".$unicod."'" );
	$fase = wf_pegarEstadoAtual($docid);

	if($fase['esdid'] == REUNI_ESTADO_IFES or $fase['esdid'] == REUNI_ESTADO_ELABORACAO){

		$sql = "select  count(a.grpcod)- count(r.prgcod) as qtd
			from
			reuni.grupopergunta gp
			inner join reuni.grupopergunta gp2 ON gp.grpcod = gp2.grpcodfil
			inner join reuni.pergunta a ON a.grpcod = gp2.grpcod
			left join (Select p.prgcod, r.unicod, r.rspdsc from reuni.pergunta p inner join
			reuni.resposta r ON r.prgcod = p.prgcod where r.unicod = '$unicod' and char_length(r.rspdsc) > 0 group by p.prgcod, r.unicod, r.rspdsc) as r
			ON r.prgcod = a.prgcod
			where gp.grpcod = $codPai and gp.prjcod = 1
			group by gp.grpcod";


		$dado = $db->pegaUm($sql);

		if($dado == 0)
		return '<img src="/imagens/check_p.gif">';
	}
	return false;
}

function filhoPreenchido($unicod,$codFilho){
	global $db;

	$docid = (integer)$db->pegaUm( "select docid from reuni.unidadeproposta where unicod = '".$unicod."'" );
	$fase = wf_pegarEstadoAtual($docid);

	if($fase['esdid'] == REUNI_ESTADO_IFES or $fase['esdid'] == REUNI_ESTADO_ELABORACAO){


		$sql = "select count(a.grpcod) - count(r.prgcod) as qtd
		from 
			reuni.grupopergunta gp 
			inner join reuni.grupopergunta gp2 ON gp.grpcod = gp2.grpcodfil 
			inner join reuni.pergunta a ON a.grpcod = gp2.grpcod
			left join (Select p.prgcod, r.unicod, r.rspdsc from reuni.pergunta p inner join 
			reuni.resposta r ON r.prgcod = p.prgcod where r.unicod = '$unicod' and char_length(r.rspdsc) > 0 group by p.prgcod, r.unicod, r.rspdsc) as r 
			ON r.prgcod = a.prgcod 
			where gp2.grpcod = $codFilho and gp2.prjcod = 1
			group by gp2.grpcod";



		$dado = $db->pegaUm($sql);

		if($dado == 0)
		return '<img src="/imagens/check_p.gif">';
	}
	return false;
}


function status($unicod){
	return true;
	global $db;
	$sql = "select coalesce(s.stsdsc,'Não Iniciado') as status  from reuni.unidadepropostastatus u inner join reuni.statusproposta s on s.stscod = u.stscod where unicod = '$unicod' order by u.upscod desc" ;
	if(($dado = $db->pegaUm($sql))!=''){
		return $dado;
	}else{
		return 'Não iniciado';
	}
}


function dadoUnidadePreenchido($unicod){
	global $db;


	$sql = "select count(*) as qtd from reuni.unidadeproposta where unicod = '$unicod' and char_length(unpend) > 0  and char_length(unpdir) > 0 and char_length(unpcar)>0 and char_length(unpsum) > 0";
	$dado = $db->pegaUm($sql);
	if($dado > 0){

		$docid = (integer)$db->pegaUm( "select docid from reuni.unidadeproposta where unicod = '".$unicod."'");
		$fase = wf_pegarEstadoAtual($docid);

		if($fase['esdid'] == REUNI_ESTADO_IFES or $fase['esdid'] == REUNI_ESTADO_ELABORACAO){
			return '<img src="/imagens/check_p.gif">';
		}
		else
		{
			return "&nbsp;";
		}
	}
	else
	{
		return false;
	}

}

function campoPreenchido($unicod,$codPergunta){
	global $db;


	$docid = (integer)$db->pegaUm( "select docid from reuni.unidadeproposta where unicod = '".$unicod."'");
	$fase = wf_pegarEstadoAtual($docid);

	if($fase['esdid'] == REUNI_ESTADO_IFES or $fase['esdid'] == REUNI_ESTADO_ELABORACAO){

		$sql = "select char_length(rspdsc) as resposta  from reuni.resposta where unicod = '$unicod' and prgcod = $codPergunta";
		$dado = $db->pegaUm($sql);
		if($dado > 0)
		return '<img src="/imagens/check_p.gif">';
	}

	if($fase['esdid'] == REUNI_ESTADO_ADHOC){
		$sql = "";
		// ADOHC
		$sql = "select
				count( * )
			from 
				reuni.resposta r inner join reuni.parecer p on r.rspcod = p.rspcod
			where
				r.unicod = '$unicod'  	and 
				r.prgcod = $codPergunta and
				p.tpacod = 2";
		if($db->pegaUm($sql) > 0)
		return '<img src="/imagens/check_p.gif">';
		else
		return '<img src="/imagens/exclui_p.gif">';
	}

	if($fase['esdid'] == REUNI_ESTADO_COMISSAO){
		$sql = "";
		// ADOHC
		$sql = "select
				count( * )
			from 
				reuni.resposta r inner join reuni.parecer p on r.rspcod = p.rspcod
			where
				r.unicod = '$unicod'  	and 
				r.prgcod = $codPergunta and
				p.tpacod = 3";
		if($db->pegaUm($sql) > 0)
		return '<img src="/imagens/check_p.gif">';
		else
		return '<img src="/imagens/exclui_p.gif">';
	}

	return false;
}

function verificaAnexo($unicod,$codPergunta){
	global $db;
	$sql = "select rsparqcod as arquivo from reuni.resposta where unicod = '$unicod' and prgcod = $codPergunta";
	$intArquivoId = $db->pegaUm($sql);
	$img = '';

	$arrArquivoBanco = pegaArquivoPeloId( $intArquivoId );

	if ( $arrArquivoBanco['arqstatus'] == 1  )
	return '<img src="/imagens/clipe.gif">&nbsp;';
	else
	return false;
}

function porcentagemDePreenchimento($unicod){
	global $db;
	
	$sql = "select count(*) as qtd from reuni.pergunta where prgobr = true";
	$qtdCampos = $db->pegaUm($sql);

	$sql ='';

	$sql = "select count(*)
		from reuni.resposta r
		inner join reuni.pergunta p on p.prgcod = r.prgcod
		where r.unicod = '$unicod' and char_length(r.rspdsc) > 0";

	$qtdCamposPreenchidos = $db->pegaUm($sql);


	if($qtdCampos > 0){
		$porcentagem = (int) floor(((100 * $qtdCamposPreenchidos) / $qtdCampos));



		$saida1 = 'Qtde. Campos:' .$qtdCampos ;
		$saida1 .= ' / Preenchidos: ' .$qtdCamposPreenchidos;


		$saida .= espaco(11).'<span style="color:cococo;font-size: 10px;">'. $porcentagem. '%</span>
		 <div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;\" title="'. $saida1 . ' (' . $porcentagem . '%)">
		 <div style="font-size:4px;width: '. $porcentagem. '%; height: 6px; max-height: 6px; background-color:#339933;">
		 </div></div>';
		return $saida;

	}

}

function procuraNoNaSessao( $strIdNo )
{
	if( isset( $_SESSION[ 'ArvoreSessaoReuni' ][ $strIdNo ] ) )
	{
		return $_SESSION[ 'ArvoreSessaoReuni' ][ $strIdNo ] ? "" : "none";
	}
	else
	{
		return "none";
	}
}

function procuraNoNaSessaoImagem( $strIdNo )
{
	if( isset( $_SESSION[ 'ArvoreSessaoReuniSrc' ][ $strIdNo ] ) )
	{
		return $_SESSION[ 'ArvoreSessaoReuniSrc' ][ $strIdNo ];
	}
	else
	{
		return "mais";
	}
}

?>
<!-- biblioteca javascript local -->
<script type="text/javascript"></script>
<!-- corpo da página -->
<script
	type="text/javascript" src="../../includes/JsLibrary/_start.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/tags/superTitle.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/_start.js"></script>
<style>
.TitleClass {
	background-color: #ffffcc;
	border: 1px solid #707070;
	color: #252525;
	font-size: 11px;
	font-weight: normal;
	padding: 3px 5px 3px 5px;
}
</style>
<table align="center" bgcolor="#f5f5f5" cellpadding="10" cellspacing="0"
	border="0" class="tabela">
	<tr>
		<!-- ESQUERDA -->
		<td>
		<table style="width: 100%;" class="tabela" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td align='right' class="SubTituloDireita">Unidade :</td>
				<td><?= nomeUnidade($_REQUEST['unicod'] ) ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Preenchimento :</td>
				<td><?= porcentagemDePreenchimento( $_REQUEST['unicod'] ) ?></td>
			</tr>
			<?php if ( reuni_podeVerResponsaveis( $_REQUEST['unicod'] ) ) : ?>
			</tr>
			<tr>
				<td colspan="2" align='right' class="SubTituloEsquerda">Responsabilidades</td>
			</tr>
			<?= $db->mostra_resp( $_REQUEST['unicod'], 'unicod', false, 'reuni' ); ?>
			<?php endif; ?>
		</table>

		<form action="" method="post" name="formulario"><?php


		$parecerGeral =	reuni_parecerGlobalPendente($_REQUEST['unicod']);



		$docid = (integer)$db->pegaUm( "select docid from reuni.unidadeproposta where unicod = '". (integer)$_REQUEST['unicod'] ."'" );
		?> <!-- AVISO DE PARECER GERAL --> <?php // if ( $parecerGeral ) : ?>
		<?php if ( reuni_podeVerAvisoParecer() ) : ?> <blink> <font
			style="color: red; font-size: 18px"><b>*</b></font> </blink> <a
			style="cursor: pointer;" onclick="parecergeral();"> <font
			style="color: red; font-size: 15px"><b> Para ir para o próximo passo,
		clique aqui e preencha o parecer geral.</font></b> </a> <?php endif; ?>
		<br>
		<br>
		<!-- QUESTÕES --> &nbsp;&nbsp;<b>1.</b> <img
			src='/imagens/offline.gif'>&nbsp;&nbsp; <a
			href="reuni.php?modulo=principal/dadounidade&acao=C&unidadeCod=<?=$_REQUEST['unicod']?>">
		<b>Dados da Unidade</b> </a> <?= dadoUnidadePreenchido( $_REQUEST['unicod'] ) ?>
		<?php
		$i = 0;
		if( dadoUnidadePreenchido( $_REQUEST['unicod'] ) )
		{
			?> <br /></br>
		&nbsp;&nbsp;<b>2.</b> &nbsp;&nbsp; <b>As dimensões do plano de
		reestruturação</b>
		<table id="trDisplay" border="0">
		<?
		while ($i < count($result))
		{
			?>
			<tr style="cursor: pointer" class="nivel1"
				id="<?= $result[$i]['codpai']?>" parent="">
				<td  style="padding-left: <?php if($result[$i]['codpai'] >= 28) echo ''; else echo'20px'?>" onclick="FechaElemento( '<?= $result[$i]['codpai']?>' )">
				<img
					src="/imagens/<?=procuraNoNaSessaoImagem(  $result[$i]['codpai'] )?>.gif" />
				<b> <?php 
				if($result[$i]['codpai'] >= 28){
					echo $result[$i]['dscpai'];
					echo paiPreenchidoQuestao($_REQUEST['unicod'],$result[$i]['codpai']);
				}else{
					echo $result[$i]['dscpai'];
					echo paiPreenchido($_REQUEST['unicod'],$result[$i]['codpai']);
				}
				?> </b></td>
			</tr>
			<?php
			$codPai     = $result[$i]['codpai'];
			$codPaiNovo = $result[$i]['codpai'];

			$t=1;
			while($codPai == $codPaiNovo )
			{
				$codFilho = $result[$i]['codfilho'];
				$codFilhoNovo = $codFilho;
					
				if($codFilho != '')
				{
					?>
			<tr  style="cursor: pointer ; display:<?=procuraNoNaSessao( $result[$i]['codpai'] . '_' . $codFilho )?>" class="nivel2" id="<?= $result[$i]['codpai'] . '_' . $codFilho ?>" parent="<?= $result[$i]['codpai']?>"  onclick="FechaElemento( '<?=  $result[$i]['codpai'] . '_' . $codFilho ?>')">
				<td style="padding-left: 30px"><img
					src="/imagens/<?=procuraNoNaSessaoImagem( $result[$i]['codpai'] . '_' . $codFilho )?>.gif" />

				<b> <?php echo $result[$i]['dscfilho']; echo filhoPreenchido($_REQUEST['unicod'],$result[$i]['codfilho'])	?>
				</b></td>
			</tr>
			<?
			$j = 1 ;
			while($codFilhoNovo == $codFilho  )
			{
				$img = '';
					
				if($result[$i]['obrpergunta'] =='t'){
					$img = "<img src='/imagens/offline.gif'>&nbsp;&nbsp;";
				}
					
				$a =  'href="/'.$_SESSION['sisdiretorio'].'/'.$_SESSION['sisarquivo'].'.php?modulo=principal/cadastroproposta&acao=C&id='.md5_encrypt($result[$i]['codpergunta'] .'_'.$result[$i]['codpai'] .'_'.$result[$i]['codfilho'] ,'').'&unicod='.base64_encode($_REQUEST['unicod']).'&len='.base64_encode($result[$i]['qtdcaracteres']).' "';
				?>
			<tr 
										style="display:<?=procuraNoNaSessao($result[$i]['codpai'] . '_' . $codFilho . '_' . $result[$i]['codpergunta']) ?>" class="nivel3" 
										id="<?= $result[$i]['codpai'] . '_' . $codFilho . '_' . $result[$i]['codpergunta'] ?>" 
										parent="<?= $result[$i]['codpai'] . '_' . $codFilho ?>">
				<td style="padding-left: 40px"><?= $img ?> <?= $j ?> <a <?= $a ?>> <?= $result[$i]['dscpergunta'] ?>
				</a> <?
				echo verificaAnexo($_REQUEST['unicod'],$result[$i]['codpergunta']);
				echo campoPreenchido($_REQUEST['unicod'],$result[$i]['codpergunta']) ;
				?></td>
			</tr>
			<?
			$j++;
			$i++;

			$codFilhoNovo = $result[$i]['codfilho'];
}
//$i++;
$codPaiNovo = $result[$i]['codpai'];
}else{
	$img = '';

	if($result[$i]['obrpergunta'] =='t'){
		$img = "<img src='/imagens/offline.gif'>&nbsp;&nbsp;";
	}

	$a = 'href="/'.$_SESSION['sisdiretorio'].'/'.$_SESSION['sisarquivo'].'.php?modulo=principal/cadastroproposta&acao=C&id='.md5_encrypt($result[$i]['codpergunta'] .'_'.$result[$i]['codpai'] .'_'.$result[$i]['codfilho'] ,'').'&unicod='.base64_encode($_REQUEST['unicod']) .'&len='.base64_encode($result[$i]['qtdcaracteres']).' "';
	?>
			<tr style="display:<?=procuraNoNaSessao($result[$i]['codpai'] . '_x_' . $result[$i]['codpergunta'])?>" class="nivel2dif" id="<?= $result[$i]['codpai'] . '_x_' . $result[$i]['codpergunta'] ?>" parent="<?= $result[$i]['codpai']?>">
				<td style="padding-left: 10px"><?= $img ?> <?= $t ?> <a <?=$a ?>> <?= $result[$i]['dscpergunta'] ?>
				</a> <?
				echo verificaAnexo($_REQUEST['unicod'],$result[$i]['codpergunta']);
				echo campoPreenchido($_REQUEST['unicod'],$result[$i]['codpergunta']);
				?></td>
			</tr>
			<?
			$t++;
			$i++;
			$codPaiNovo = $result[$i]['codpai'];
}
}
}
?>
			</td>
			</tr>
			<tr>
				<td>
					
					<br>
					<!-- botão para acionar relatorio plano de aplicação -->
					<!-- 
					<input
						type="button" class="botao"
						value="Plano Aplicação"
						onclick="relatorioPlanoAplicacao( '<?= $_SESSION['unicod'] ?>' );"
					/>
					
					<br> -->
					<!-- botão para acionar relatorio plano de aplicação global-->
					<input
						type="button" class="botao"
						value="Plano Aplicação Global"
						onclick="relatorioPlanoAplicacaoGlobal( '<?= $_SESSION['unicod'] ?>' );"
					/>
					
					<!-- botão para acionar relatorio plano de aplicação 2007-->
					<?
					
						$bloqueados = array('26230', '26231', '26247', '26248', '26255', '26258', '26260', '26274', '26277', '26351', '26261');
						
						if($_SESSION['superuser']){
							
						$bloqueados = array();	
							
						}
						
						if ( ! in_array($_SESSION['unicod'], $bloqueados)){	
							?>
							<br/>
								<!-- botão para acionar relatorio plano de aplicação 2007 -->
								<input class="botao"
									type="button"
									value="Plano Aplicação 2007"
									onclick="relatorioPlanoAplicacao60( '<?= $_SESSION['unicod'] ?>' );"
								/>
							<?
							 
						}
					?>
					
					<br/>
					<!-- botão para acionar relatorio plano de aplicação 2008-->
					<!--<input class="botao"
						type="button"
						value="Plano Aplicação 2008"
						onclick="relatorioPlanoAplicacao2008( '<?= $_SESSION['unicod'] ?>' );"
					/>
					
					
					
					--><?if(wf_acaoFoiExecutada($docid,REUNI_ESTADO_IFES,REUNI_ESTADO_SESU)){?>
						<br>
						<br>
						<input type="button" value="Parecer Geral" onclick="parecergeral();" />
					<?}?>


				<p>&nbsp;</p>
				<p>&nbsp;</p>
				</td>
			</tr>
		</table>
		<?php
}
else
{
	echo espaco( 5 ) . "<b>Para prosseguir com o cadastro, preencha os dados da Unidade.</b>";
}
?></form>
		</td>

		<!-- DIREITA -->
		<td align="center" valign="top"><!-- WORKFLOW --> <?php wf_desenhaBarraNavegacao( $docid, array( "unicod" => $_REQUEST['unicod'] ) ); ?>

		<!-- AÇÕES -->
		<table border="0" cellpadding="3" cellspacing="0"
			style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
			<tr style="background-color: #c9c9c9; text-align: center;">
				<td style="font-size: 7pt; text-align: center;"><span
					title="estado atual"> <b>ferramentas</b> </span></td>
			</tr>
			<?php if ( dadoUnidadePreenchido( $_REQUEST['unicod'] ) ) : ?>
			<tr style="text-align: center;">
				<td style="font-size: 7pt; text-align: center;"><a
					style="cursor: pointer" onclick="versao_impressao();"> <img
					src="/imagens/print.png" alt="Versão para impressão"
					onclick="versao_impressao();"
					onmouseover="window.SuperTitleOn( this , 'Clique aqui para ver a versão para impressão.' )">
				</a></td>
			</tr>
			<?php endif; ?>
			<?php if ( reuni_podeEditarSimulador( $_REQUEST['unicod'] ) ) : ?>
			<tr style="text-align: center;">
				<td style="font-size: 7pt; text-align: center;"><a
					style="cursor: pointer;" onclick="logarReuni()"
					onmouseover="window.SuperTitleOn( this , 'Abrir o Simulador de Cálculo do Projeto' )"
					onmouseout="window.SuperTitleOff( this )"> <img
					src="/imagens/excel.gif" alt="Simulador"> </a></td>
			</tr>
			<?php endif; ?>
			<?php if ( reuni_podeVerPdfSimulador( $_REQUEST['unicod'] ) ) : ?>
			<tr style="text-align: center;">
				<td style="font-size: 7pt; text-align: center;"><a
					href="http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/simulador/index.php?view=planilhasPDF">
				<img border="0" src="/imagens/acrobat.gif"
					onmouseover="window.SuperTitleOn( this , 'Gerar Quadro Síntese de Indicadores Acadêmicos e de Orçamento.' )">
				</a></td>
			</tr>
			<?php endif; ?>
			<?php if ( dadoUnidadePreenchido( $_REQUEST['unicod'] ) ) : ?>
			<tr style="text-align: center;">
				<td style="font-size: 7pt; text-align: center;"><a
					href="http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/imagens/processo-reuni.gif"
					target="_blank"> <img border="0" src="/imagens/fluxodocgraf.gif"
					alt="Visualizar Fluxo de Processo."
					onmouseover="window.SuperTitleOn( this , 'Visualizar Fluxo de Processo.' )">
				</a></td>
			</tr>
			<?php endif; ?>
		</table>

		</td>
	</tr>
</table>


<script type="text/javascript">
	
	// funcao para acionar relatorio plano de aplicação
	function relatorioPlanoAplicacao( unicod )
	{
		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/relatorioplanoaplicacao&acao=C&unicod=' + unicod,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	}
	
	// funcao para acionar relatorio plano de aplicação global
	function relatorioPlanoAplicacaoGlobal( unicod )
	{
		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/relatorioglobal&acao=C&unicod=' + unicod,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	}
	
	// funcao para acionar relatorio plano de aplicação 2007
	function relatorioPlanoAplicacao60( unicod )
	{
		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/relatorio60&acao=C&unicod=' + unicod,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	}
	
	// funcao para acionar relatorio plano de aplicação 2008
	function relatorioPlanoAplicacao2008( unicod )
	{
		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/relatorio2008&acao=C&unicod=' + unicod,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	}
	
	var dav = navigator.appVersion ;
	IE = document .all ? true : false;
	
	
	function reloadPage(){
		window.location.reload();
	}
	
	function parecergeral()
	{		
		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/reuni/parecergeral.php?unicod=<?=base64_encode($_REQUEST['unicod'])?>','blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	}
	
	
	function versao_impressao()
	{		
		window.open('reuni.php?modulo=principal/versaoimpressao&acao=C&unicod=<?=base64_encode($_REQUEST['unicod'])?>','blank','width=800,height=600,status=0,toolbar=0,location=0,scrollbars=1,resizable=1');	
	}
	
	function historicoTramitacoes()
	{
		window.open('reuni.php?modulo=principal/tramitacao&acao=C&unicod=<?=base64_encode($_REQUEST['unicod'])?>','blank','height=300,width=600,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	}
	
	
	function logarReuni()
	{
		window.open('/reuni/simulador/index.php','blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
	}
	
	
	function FechaElemento( strIdElemento , _boolFecha )
	{
		var objElemento = document.getElementById( strIdElemento );
		var arrAlterados = Array();
		var boolPrimeiraChamada = false;
		if( _boolFecha == undefined )
		{
			boolPrimeiraChamada = true;
			var objImg = objElemento.getElementsByTagName( "img" )[0];
			var strSrc = objImg.src;
			_boolFecha = ( strSrc.indexOf( "menos" ) > 0 );
			var strPedaco;
			if( _boolFecha )
			{
				strSrc = strSrc.replace( "menos" , "mais" );
				strPedaco = "mais";
	
			}
			else
			{
				strSrc = strSrc.replace( "mais" , "menos" );
				strPedaco = "menos";
			}
			objImg.src = strSrc;
			arrParams = Array( objElemento.id , strPedaco );
			addRequest( "../atualizaArvoreSessaoReuni.php" , "mudaImg" , arrParams, function(){}  );
	
		}
		arrTrElementos = ( objElemento.parentNode.getElementsByTagName( "tr" ) );
		var strEvento = _boolFecha ? "fechaNo" : "mostraNo";
		var arrParams = Array();
	
		for( var i = 0; i < arrTrElementos.length; i++ )
		{
			var objTrElemento = arrTrElementos[ i ];
			if( objTrElemento.getAttribute( "parent" ) == objElemento.id )
			{
				if( _boolFecha )
				{
					arrParamsFilhos = FechaElemento( objTrElemento.id , _boolFecha );
					arrParams = arrParams.concat( arrParamsFilhos );
				}
				else
				{
					if( objTrElemento.getElementsByTagName( "img").length > 0 )
					{
						var objImg = objTrElemento.getElementsByTagName( "img" )[0];
						var strSrc = objImg.src;
						strSrc = strSrc.replace( "menos" , "mais" );
						objImg.src = strSrc;
					}
				}
				var parametro;
				if(IE)
				parametro =  "block";
				else
				parametro = "table-row";
	
				objTrElemento.style.display = _boolFecha ? "none" : parametro;
	
				arrParams.push( objTrElemento.id );
			}
		}
	
		if( boolPrimeiraChamada )
		{
			addRequest( "../atualizaArvoreSessaoReuni.php" , strEvento , arrParams, function(){}  );
		}
		return arrParams;
	}
	
	    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }

</script>
