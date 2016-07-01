<?php

function recarregar_arvore(){
	$location = $_SESSION['sisdiretorio'] . ".php?modulo=" . $_REQUEST['modulo'] . "&acao=" . $_REQUEST['acao'];
	?>
	<script type="text/javascript">
	  	location.href = "<?= $location ?>";
	</script>
	<?php
	exit();
}

function excluir_registro( $atiid ) {
	global $db;
	// captura as informações da atividade a ser excluída
	$sql = sprintf( "select * from pde.atividade a where a.atiid = %s and a.atistatus = 'A'", $atiid );
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// exclui a atividade
	$sql = sprintf( "update pde.atividade set atistatus = 'I' where atiid = %s", $atividade['atiid'] );
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	// reordena as atividades que tem o mesmo pai
	$sql = sprintf(
		"update pde.atividade set atiordem = atiordem - 1 where atiidpai = %s and atiordem > %s and atistatus = 'A'",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	return true;
}

function pegar_filhos( $registro = null ){
	global $db;
	$sql = sprintf(
		"select * from pde.atividade where atistatus='A' and atiidpai %s order by atiordem",
		$registro ? " = ". $registro['atiid'] : " is null "
	);
	$filhos = $db->carregar( $sql );
	if ( !$filhos ) {
		return array();
	}
	return $filhos;
}

function pegar_anexo( $id ){
	global $db;
	$sql = sprintf(
		"select distinct ta.taadescricao from pde.anexoatividade at inner join pde.tipoanexoatividade ta on at.taaid=ta.taaid where anestatus='A' and atiid %s",
		$id ? " = ". $id : " is null "
	);
	$linhas = $db->carregar( $sql );
	if ( !$linhas ) {
		//$anexos.="<ul>";
	} else {
		
		foreach ( $linhas as $linha ) 
		{			
			$anexos.="- ".$linha['taadescricao']."<br>";
		}
		//$anexos.="</ul>";
		return $anexos;
	}
	
}

function pegar_controle( $id ){
	global $db;
	$sql = sprintf(
		"select ob.obsdescricao from pde.observacaoatividade ob where ob.obsstatus='A' and ob.atiid %s order by obsid desc limit 1",
		$id ? " = ". $id : " is null "
	);
	$linhas = $db->carregar( $sql );
	if ( !$linhas ) {
		return "-";
	} else {
		foreach ( $linhas as $linha ) 
		{			
			$anexos.=$linha['obsdescricao'];
		}
		return $anexos;
	}
	
}


function pegar_orcamento( $id, $anoini, $anofim ){
	global $db;
	$sql = sprintf(
		"select sum(orcvalor) as valor from pde.orcamentoatividade where orcano>='$anoini' and orcano<='$anofim' and atiid %s ",
		$id ? " = ". $id : " is null "
	);
	$linha = $db->pegaLinha( $sql );

	if ( !$linha ) {
		return "-";
	} else {
			if ($linha['valor']>0) 
					{
						return number_format($linha['valor'],0,',','.');
					} 
				else 
					{
						return "-";
					}
	}
	
}

function exibir_registro( $registro, $nivel = 0, $rastro = '', $total ){
	static $numero = 0;
	static $cor = '#f7f7f7';
	static $profundidade = 10; # nivel a partir do qual os itens serão ocultados
	
	$cor = ( $cor == '#f7f7f7' ) ? '#ffffff' : '#f7f7f7';
	if ( !$registro ) {
		return;
	}
	$numero++;
	$filhos = pegar_filhos( $registro );
	?>
	<tr id="tr<?=$registro['atiidpai']?>" bgcolor="<?= $cor ?>" onmouseout="this.bgColor='<?= $cor ?>';" onmouseover="this.bgColor='#ffffcc';" parent="<?= $registro['atiidpai'] ?>">
		<td align="right" nowrap style="width:15px; text-align:rigth"><?= $numero ?> </td>
		<td nowrap style="width:70px; text-align:center" class="notprint">
			<img border="0" src="../imagens/gif_inclui.gif" onclick="cadastrar( <?= $registro['atiid'] ?> )" title="Cadastrar Atividade" onmouseover="this.style.cursor='pointer'"/>
			<img border="0" src="../imagens/alterar.gif" onclick="editar( <?= $registro['atiid'] ?> )" title="Editar Atividade" onmouseover="this.style.cursor='pointer'"/>
			<img border="0" src="../imagens/excluir.gif" onclick="excluir( <?= $registro['atiid'] ?> )" title="Excluir Atividade" onmouseover="this.style.cursor='pointer'"/>
		</td>
		<td nowrap style="width:50px; text-align:center"" class="notprint">
			<?php if( $nivel != 0 ): ?>
				<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&botao=esquerda&atiid=<?= $registro['atiid'] ?>">
					<img title="Recuo Esquerda" src="../imagens/recuo_e.gif" style="border:0"/>
				</a>
			<?php else: ?>
				<img title="" src="../imagens/recuo_e_d.gif" style="border:0"/>
			<?php endif; ?>
			
			<?php if( $registro['atiordem'] > 1 ): ?>
				<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&botao=direita&atiid=<?= $registro['atiid'] ?>">
					<img title="Recuo Direita" src="../imagens/recuo_d.gif" style="border:0"/>
				</a>
			<?php else: ?>
				<img title="" src="../imagens/recuo_d_d.gif" style="border:0"/>
			<?php endif; ?>
		</td>
		<td style="padding-left: <?= $nivel * 25 ?>px" class="coluna_medida">
			<?php if( $nivel > 0 ): ?>
				<img src="../imagens/seta_filho.gif" align="left"/>
			<?php endif; ?>
			<div style="float:left; width: 90%;<?= count( $filhos ) > 0 ? 'font-weight: bold' : '' ?>">
				<?if (count( $filhos ) > 0) {?>
					<img id="img<?= $registro['atiid'] ?>" atividade="<?= $registro['atiid'] ?>" onclick="exibirOcultarAtividadesFilhas( <?= $registro['atiid'] ?>, this, true );" id="imgTarefa<?= $registro['atiidpai'] ?>" src="../imagens/menos.gif"/>&nbsp;
				<?}?>
				<span onmouseover="this.style.cursor='pointer'" onclick="editar( <?= $registro['atiid'] ?> )"><?= $rastro . $registro['atiordem'] .' - '. $registro['atidescricao'] ?></span>
			</div>
		</td>
		<td class="coluna_instrumento" onclick="anexar( <?= $registro['atiid'] ?> )" onmouseover="this.style.cursor='pointer'"><?=pegar_anexo( $registro['atiid'] )?></td>
		<td class="coluna_meta"><?= $registro['atimeta'] ?></td>
		<td><?= $registro['atiinterface'] ?></td>
		<td class="coluna_orcamento" onclick="orcamento( <?= $registro['atiid'] ?> , 2007 )" onmouseover="this.style.cursor='pointer'"><?=pegar_orcamento($registro['atiid'],'2007','2007')?></td>
		<td class="coluna_orcamento" onclick="orcamento( <?= $registro['atiid'] ?> , 2007, 2011 )" onmouseover="this.style.cursor='pointer'"><?=pegar_orcamento($registro['atiid'],'2008','2010')?></td>
		<td class="coluna_controle" onclick="apenasControle( <?= $registro['atiid'] ?> )" onmouseover="this.style.cursor='pointer'"><?=pegar_controle( $registro['atiid'] )?></td>
		<td class="coluna_data"><?= formata_data( $registro['atidatainicio'] ) ?></td>
		<td class="coluna_data"><?= formata_data( $registro['atidatafim'] ) ?></td>
		<td nowrap style="width:50px; text-align:center"" class="notprint">
			<?php if( $registro['atiordem'] != 1 ): ?>
				<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&botao=subir&atiid=<?= $registro['atiid'] ?>">
					<img title="Mover para cima" src="../imagens/seta_cima.gif" style="border:0"/>
				</a>
			<?php else: ?>
				<img title="" src="../imagens/seta_cimad.gif" style="border:0"/>
			<?php endif; ?>
			
			<?php if( $registro['atiordem'] != $total ): ?>
				<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&botao=descer&atiid=<?= $registro['atiid'] ?>">
					<img title="Mover para baixo" src="../imagens/seta_baixo.gif" style="border:0"/>
				</a>
			<?php else: ?>
				<img title="" src="../imagens/seta_baixod.gif" style="border:0"/>
			<?php endif; ?>
		</td>
	</tr>
	<?php
	$rastro .= $registro['atiordem'] . '.';
	foreach ( $filhos as $filho ) {
		exibir_registro( $filho, $nivel + 1, $rastro, count( $filhos ) );
	}
	?>
	<?php if( $nivel >= $profundidade ): ?>
		<script type="text/javascript">
			var imagem = document.getElementById( "img<?= $registro['atiid'] ?>" );
			if ( imagem ) {
				imagem.onclick();
			}
		</script>
	<?php endif; ?>
	<?
}

switch( $_REQUEST['acao'] ){
	case 'E':
		$sql = "select count(*) as total from pde.atividade where atistatus='A' and atiidpai=".$_REQUEST['atiid'];
		$qtdfilhos = $db->pegaUm( $sql );
		if ($qtdfilhos>0) {
			print "<script>alert('Não foi possível excluir, exclua primeiramente as atividades filhas!');history.back();</script>";
			exit();
		} else {
			if( !excluir_registro( $_REQUEST['atiid'] ) ) {
				$db->rollback();
				$db->insucesso( 'Ocorreu um erro, a exclusão foi cancelada.', array(), $modulo );
			}
			$db->commit();
			$_REQUEST['acao'] = 'C'; # gambiarra
			$db->sucesso( $modulo );
		}
		break;
	default:
		break;
}


switch( $_REQUEST['botao'] ){
	case 'subir':
		$sql = sprintf(
			"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A' and a1.atiordem > 1",
			$_REQUEST['atiid']
		);
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			break; # é o primeiro, não faz nada
		}
		$sql = sprintf(
			"update pde.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
			$atividade['atiordem'],
			$atividade['atiordem'] - 1,
			$atividade['atiidpai']
		);
		if( !$db->executar( $sql ) ) {
			$db->insucesso( 'Ocorreu um erro, a operação foi cancelada.', array(), $modulo );
		}
		$sql = sprintf(
			"update pde.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
			$atividade['atiordem'] - 1,
			$atividade['atiid']
		);
		if( !$db->executar( $sql ) ) {
			$db->insucesso( 'Ocorreu um erro, a operação foi cancelada.', array(), $modulo );
		}
		$db->commit();
		recarregar_arvore();
		break;
	case 'descer':
		$sql = sprintf(
			"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A' and a1.atiordem < ( select count(*) from pde.atividade a2 where a2.atiidpai = a1.atiidpai and atistatus = 'A' )",
			$_REQUEST['atiid']
		);
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			break; # é o último, não faz nada
		}
		$sql = sprintf(
			"update pde.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
			$atividade['atiordem'],
			$atividade['atiordem'] + 1,
			$atividade['atiidpai']
		);
		if( !$db->executar( $sql ) ) {
			$db->insucesso( 'Ocorreu um erro, a operação foi cancelada.', array(), $modulo );
		}
		$sql = sprintf(
			"update pde.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
			$atividade['atiordem'] + 1,
			$atividade['atiid']
		);
		if( !$db->executar( $sql ) ) {
			$db->insucesso( 'Ocorreu um erro, a operação foi cancelada.', array(), $modulo );
		}
		$db->commit();
		recarregar_arvore();
		break;
	case 'esquerda':
		// carrega os dados da atividade
		$sql = sprintf(
			"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
			$_REQUEST['atiid']
		);
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			break; # a atividade não existe
		}
		// carrega os dados do antigo pai da atividade
		$sql = sprintf(
			"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
			$atividade['atiidpai']
		);
		$atividade_pai = $db->pegaLinha( $sql );
		if ( !$atividade_pai ) {
			break; # a atividade não existe
		}
		// desloca os novos irmãos para baixo
		$sql = sprintf(
			"update pde.atividade set atiordem = atiordem + 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
			$atividade_pai['atiidpai'],
			$atividade_pai['atiordem']
		);
		if ( !$db->executar( $sql ) ) {
			break;
		}
		// desloca os antigos irmãos para cima
		$sql = sprintf(
			"update pde.atividade set atiordem = atiordem - 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
			$atividade['atiidpai'],
			$atividade['atiordem']
		);
		if ( !$db->executar( $sql ) ) {
			break;
		}
		// troca o pai (pelo avô)
		$sql = sprintf(
			"update pde.atividade set atiidpai = %d, atiordem = %d where atistatus = 'A' and atiid = %d",
			$atividade_pai['atiidpai'],
			$atividade_pai['atiordem'] + 1,
			$atividade['atiid']
		);
		if ( !$db->executar( $sql ) ) {
			break;
		}
		$db->commit();
		recarregar_arvore();
		break;
	case 'direita':
		// carrega os dados da atividade
		$sql = sprintf(
			"select a1.* from pde.atividade a1 where atiid = %d and atistatus = 'A'",
			$_REQUEST['atiid']
		);
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			dbg( 1, 1 );
			break; # a atividade não existe
		}
		// carrega o novo pai (irmão que está uma posição acima)
		$sql = sprintf(
			"select a1.* from pde.atividade a1 where atiidpai = %d and atiordem = %d and atistatus = 'A'",
			$atividade['atiidpai'],
			$atividade['atiordem'] - 1
		);
		$atividade_pai = $db->pegaLinha( $sql );
		if ( !$atividade_pai ) {
			dbg( $sql, 1 );
			break; # a atividade não existe
		}
//		// desloca todos os novos irmãos para baixo
//		$sql = sprintf(
//			"update pde.atividade set atiordem = atiordem + 1 where atiidpai = %d and atistatus = 'A'",
//			$atividade_pai['atiid']
//		);
//		if ( !$db->executar( $sql ) ) {
//			dbg( 1, 1 );
//			break;
//		}
		// desloca os antigos irmãos para cima
		$sql = sprintf(
			"update pde.atividade set atiordem = atiordem - 1 where atiidpai = %d and atiordem > %d and atistatus = 'A' ",
			$atividade['atiidpai'],
			$atividade['atiordem']
		);
		if ( !$db->executar( $sql ) ) {
			dbg( 1, 1 );
			break;
		}
		// troca o pai (pelo antigo irmão)
		$sql = sprintf(
			"update pde.atividade set atiidpai = %d, atiordem = 1 + ( select count(*) from pde.atividade where atiidpai = %d and atistatus = 'A' ) where atiid = %d and atistatus = 'A'",
			$atividade_pai['atiid'],
			$atividade_pai['atiid'],
			$atividade['atiid']
		);
		if ( !$db->executar( $sql ) ) {
			dbg( 1, 1 );
			break;
		}
		$db->commit();
		recarregar_arvore();
		break;
}

?>
	<?php require_once APPRAIZ . "includes/cabecalho.inc"; ?>
	<style>
		.coluna_medida
		{
			color:#333333;
			width:40%;
			cursor: poiter;
			
		}
		
		.coluna_data
		{
			color:#4488CC;
			
		}
		
		.coluna_meta
		{
			color:#008000;
			text-align: center;
		}
		
		.coluna_instrumento
		{
			color: #2277bb;
			text-align: left;
			font-size: 10px;
		}
	
		.coluna_orcamento
		{
			color:#3366CC;
			text-align: right;
		}	
		
		.coluna_controle
		{
			text-align: center;
			color:#CC9933;
		}	
	</style>
	<script type="text/javascript">
		
		function anexar( atividade )
		{
			window.open(
				"pde.php?modulo=principal/atividade/alterar&acao=A&atiid=" + atividade + "&evento=anexar",
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		function editar( atividade ){
			window.open(
				"pde.php?modulo=principal/atividade/alterar&acao=A&atiid=" + atividade + "&evento=editar",
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		function excluir(id){
			location.href = "pde.php?modulo=principal/atividade/gerenciar&acao=E&atiid="+id
		}
		
		function cadastrar( atividade ){
			window.open(
				"pde.php?modulo=principal/atividade/cadastrar&acao=C&atiidpai=" + atividade,
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		function mostra(id, imgelemento){
			//alert ("tr"+id);
			//alert (document.getElementById("t"+id).innerHTML);
			document.getElementById("t"+id).innerHTML='';
		}
		
		function apenasControle( atividade ){
			window.open(
				"pde.php?modulo=principal/atividade/alterar&acao=A&atiid=" + atividade + "&evento=controle" ,
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		function orcamento( atividade , ano ){
			window.open(
				"pde.php?modulo=principal/atividade/alterar&acao=A&atiid=" + atividade + "&evento=orcamento&ano=" + ano ,
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		function popup_arquivo( acao, nome, width, height )
		{
			window.open( '../geral/popup_arquivoupload.php?acao=' + acao + '&nome=' + nome, '', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
		}
		
		function abredocumento(verid)
		{
			alert('Abre documento: '+verid);
		}

		function mover_cima(id)
		{
			alert('Para Cima: '+id);
		}
		
		function mover_baixo(id)
		{
			alert('Para Baixo: '+id);
		}
		
		var IE = document.all ? true : false;
		
		function exibirOcultarAtividadesFilhas( atividade, imagem, origem ){
			var atividades = document.getElementById( 'atividades' ).getElementsByTagName( 'tr' );
			for( var i = 0; i < atividades.length ; ++i ) {
				if( atividades[i].getAttribute( 'parent' ) == atividade ) {
					if ( atividades[i].style.display == "none" ) {
						if( !IE ) {
							atividades[i].style.display = "table-row";
						} else {
							atividades[i].style.display = "block";
						}
						if ( origem == true ) {
							imagem.src = imagem.src.replace( 'mais' , 'menos' );
						}
					} else {
						atividades[i].style.display = "none";
						if ( origem == true ) {
							imagem.src = imagem.src.replace( 'menos' , 'mais' );
						}
					}
					var imagens = atividades[i].getElementsByTagName( 'img' );
					for( var j = 0; j < imagens.length ; ++j ) {
						if( imagens[j].getAttribute( 'atividade' ) != null && imagens[j].src.indexOf( 'menos' ) > 0 ) {
							exibirOcultarAtividadesFilhas( imagens[j].getAttribute( 'atividade' ), imagens[j], false );
						}
					}
				}
			}
		}
	
	</script>
	<br/>
	<?php monta_titulo( 'PDE - Plano de Desenvolvimento da Educação', 'Escolha uma ação desejada: <img border="0" align="absmiddle" src="../imagens/alterar.gif"/> Alterar <img border="0" align="absmiddle" src="../imagens/excluir.gif"/> Excluir <img border="0" align="absmiddle" src="../imagens/gif_inclui.gif"/> Incluir' ); ?>
	<table id="atividades" width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color: rgb(51, 51, 51);border-top:none;">
		<thead>
			<tr style="text-align: center">
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">nº</td>
				<td valign="top" class="title notprint" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Ações</td>
				<td valign="top" class="title notprint" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Recuo</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Medida</td>
				<td align="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Instrumento</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Meta</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Interface</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">2007</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">2007-2011</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Controle</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Início</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Fim</td>
				<td valign="top" class="title notprint" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Ordem</td>
			</tr>
		</thead>
		<tbody>
			<?php
				$raiz = array(
					'atiid' => 3
				);
				$filhos = pegar_filhos( $raiz );
				foreach ( $filhos as $registro ) {
					exibir_registro( $registro, 0, '', count( $filhos ) );
				}
			?>
		</tbody>
		<tfoot>
			<tr>
				<td colspan="13" style="background-color:#ffffff; text-align:center; padding:15px">
					<div onclick="cadastrar( <?= $raiz['atiid'] ?> )" title="Cadastrar Atividade" onmouseover="this.style.cursor='pointer'">
						<img border="0" src="../imagens/gif_inclui.gif" style="vertical-align:middle;"/>
						Cadastrar Atividade
					</div>
				</td>
			</tr>
		</tfoot>
	</table>