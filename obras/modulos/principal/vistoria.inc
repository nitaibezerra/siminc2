<?php
unset($_SESSION['supvid']);


ini_set( "memory_limit", "100M" );

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

if( $_REQUEST["obrid"] ){
	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	unset($_SESSION["obra"]);
	$_SESSION['obra']['obrid'] = $_REQUEST["obrid"];
}

if($_SESSION['obra']['obrid']){
	$situacao_obra = obras_pega_situacao_vistoria( $_SESSION['obra']['obrid'] );
}else{
	echo "<script>
			alert('ID da obra não encontrado. Acesse a obra novamente!');
			history.back(-1);
		</script>";
}


//regra para permitir alteracao de dados da vistoria caso usuario com perfil de empresa,
//e o workflow esteja na situacao devolvido para empresa para ajustes.
$inclusaoEmpresaObraFinalizada = false;

if ($situacao_obra == FINALIZADA)
{
	if ( possuiPerfil(PERFIL_EMPRESA) )
	{
		$esdid = recuperaSituacaoObra( $_SESSION['obra']['obrid'] );

		if ($esdid == OBRAAJUSTESUPERVISAO_EMPRESA || $esdid == OBRAREAJUSTESUPERVISAO_EMPRESA)
		{
			$habilitado = true;
			$inclusaoEmpresaObraFinalizada = true;
		}
	}

}
//***********************************************************


$obras = new Obras();
$dobras = new DadosObra(null);

// @TODO Verificar para adaptar
// Executa as funções da tela

if( $_REQUEST["subacao"] == "VerificaVistoria" ){
	
	// Verifica se existe vistorias com data maior do que a da que esta tentando excluir
	$pode_excluir_vistoria = $obras->VerificaExistenciaVistorias( $_REQUEST["supvid"] );
	$somenteLeitura = obras_podeatualizarvistoria($_REQUEST["supvid"]);

	if( $pode_excluir_vistoria && $somenteLeitura == 'S'){
		
		$boExcluida = $obras->DeletarVistoria( $_REQUEST["supvid"] );
		if ( true !== $boExcluida ){
			echo "<script>
				 alert('" . $boExcluida . "');
				 history.back(-1);
			  </script>";
		}
		
	}else{
		
		echo "<script>
				 alert('Você não possui permissão para excluir esta vistoria!');
				 history.back(-1);
			  </script>";
			
	}
	
}


if($_SESSION["obra"]["obrid"]){
	$dados = $obras->Dados($_SESSION["obra"]["obrid"]);
	$dobras = new DadosObra($dados);
} else {
	die("<script>
			alert('Problemas nas variáveis. Refaça o procedimento.');
			window.location='obras.php?modulo=inicio&acao=A';
		 </script>");      
}

?>

<br/>

<?php

montaAbaObras($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Vistoria' );

echo $obras->CabecalhoObras();

?>
<script src="/includes/prototype.js"></script>
<form name="formulario" method="post" onSubmit="return Validacao();" action="<?php echo $caminho_atual;?>&acao=A"> 
	<input type="hidden" name="requisicao" value="executar"/> 
	<?
	echo montaVistoriaCopiaOriginal( $_SESSION["obra"]["obrid"] );
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<th>Vistoria da Obra</th>
		</tr>
		<tr>
			<td>
			<?php			
				
				function verificaValidacaoVistoria($supvid)
				{
					
					// regra para verificar se há validação da obra e bloquear alteracao de supervisoes com datas inferiores
					global $db;
					if ($_SESSION["obras"]["orgid"] ==  ORGAO_FNDE)
					{
						
						$sql = "select supvdt from obras.supervisao where supvid = ".$supvid;
						$dado = $db->pegaLinha($sql);
						$supvdt = $dado['supvdt'];
					
						$sql = "select vlddtinclusaost25exec, vlddtinclusaost50exec, vldstatus25exec, vldstatus50exec from obras.validacao
						where obrid = {$_SESSION["obra"]["obrid"]}";
						$dado = $db->pegaLinha($sql);
					

						if ($dado['vlddtinclusaost25exec'] && $dado['vldstatus25exec']=='S')
						{
							if ($supvdt <= $dado['vlddtinclusaost25exec'])
							{
								return false;
							}
						}
						if ($dado['vlddtinclusaost50exec'] && $dado['vldstatus50exec']=='S')
						{
							if ($supvdt <= $dado['vlddtinclusaost50exec'])
							{
								return false;
							}
						}
					}
					
					return true;
					//*********************************************
				}
			
				$stWhere = '';
				/*
				if($_SESSION['obras']['orgid'] == ORGAO_FNDE){
					$stWhere = " and si.stoid not in (6,7)";	
				}
				*/
				
				$stSql = "
					SELECT 
						to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
						to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,						
						COALESCE(t.traseq || ' - ' || t.tradsc, '-') AS aditivo,
						s.supvid,
						u.usunome,
						si.stodesc,
						rs.rsudsc as responsavel,
						UPPER(e.entnome) as vistoriador,
						s.supvid,
						s.usucpf,
						e.entnumcpfcnpj as cpfvistoriador,
						s.supdtinclusao,
						coalesce((SELECT 
									sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
								  FROM 
								  	obras.itenscomposicaoobra i
								  INNER JOIN 
								  	obras.supervisaoitenscomposicao si ON i.icoid = si.icoid 
								  WHERE si.supvid = s.supvid 
								  	AND obrid = {$_SESSION['obra']['obrid']} 
								  	/*AND i.icovigente = 'A'*/ ),'0') as percentual
					FROM
						obras.supervisao s
					LEFT JOIN(SELECT 
								DISTINCT
								t.traseq,
								t.tradsc,
								sic.supvid
							  FROM 
								obras.termoaditivo t
								JOIN obras.itenscomposicaoobra ico ON t.traid = ico.traid
								JOIN obras.supervisaoitenscomposicao sic ON sic.icoid = ico.icoid 
							  WHERE
								t.obrid = " . $_SESSION["obra"]["obrid"] . " 
							 ) t ON t.supvid = s.supvid	
					INNER JOIN 
						obras.situacaoobra si ON si.stoid = s.stoid
					INNER JOIN
						seguranca.usuario u ON u.usucpf = s.usucpf
					LEFT JOIN
						entidade.entidade e ON e.entid = s.supvistoriador
					LEFT JOIN
						obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
						
					--inner join
					LEFT JOIN
						obras.itenscomposicaoobra i on i.obrid = {$_SESSION["obra"]["obrid"]}
					--inner join
					LEFT JOIN
						obras.itenscomposicao ic on i.itcid = ic.itcid						
						
					WHERE
						s.obrid = '" . $_SESSION["obra"]["obrid"] . "' AND
						s.supstatus = 'A'
						
						and ( i.icovigente='A' OR i.icovigente IS NULL ) 
					{$stWhere}
					
					group by
                    	s.supvdt,
						s.supdtinclusao,						
						t.traseq,
                        t.tradsc,
						s.supvid,
						u.usunome,
						si.stodesc,
						rs.rsudsc,
						e.entnome,
						s.supvid,
						s.usucpf,
						e.entnumcpfcnpj,
						s.supdtinclusao						
					/*ORDER BY 
						s.supdtinclusao ASC*/						
					ORDER BY
						s.supvdt ASC";
				
				$dados = $db->carregar( $stSql );
				$cabecalho = array(	"Ação",
									"Possui Foto",	
									"Aditivo Vinculado",
									"Código",
									"Ordem",
									"Data Vistoria",
									"Data Inclusão",
									"Inserido Por",
									"Situação da Obra",
									"Vistoriador",
									"Realizada Por",
									"% da Vistoria"); 
			?>
			<table class="tabela" align="center" style="width: 98%;">
				<tr>
				<? $i = 1; ?>
				<? if(is_array($cabecalho)){ ?>
				<? foreach($cabecalho as $indices => $titulos){ ?>
					<th><? echo  $titulos  ?></th>
				<? }} ?>
				
				</tr>
				<?php
				if(is_array($dados)){ 
					$stBotaoExcluir = '<img src="/imagens/excluir_01.gif" border="0" title="Excluir"> ';
					//$stBotaoExcluir = '';
					$stBotaoExcluirHabilitado = '<img src="/imagens/excluir.gif" border="0" title="Excluir" style="cursor:pointer;" onclick="javascript:ExcluirVistoria(\'' . $caminho_atual . 'acao=A\', %s );">';
					$inRegistros = count( $dados );
					
					//pega ultima vistoria
					$ultimaDataVistoria = $db->pegaUm("select to_char(max(supvdt),'DD/MM/YYYY') from obras.supervisao where supstatus = 'A' and obrid = ".$_SESSION["obra"]["obrid"]);
					
					foreach($dados as $chave){ 
					//SQL que recupera os arquivos de Fotos de uma Vistoria.
					$fotoSql = "SELECT DISTINCT	
										fot.supvid 
								FROM 
									obras.fotos AS fot
								LEFT JOIN 
									public.arquivo AS arq ON arq.arqid = fot.arqid
								WHERE 
									obrid = " . $_SESSION["obra"]["obrid"] . " 
									AND supvid = " . $chave['supvid'] ;
	
					$possuiFotos = $db->carregarColuna( $fotoSql );
					
					$bgcolortr = "bgcolor=#ffffff";
				?>
							
				<tr <? if($i%2) $bgcolortr = "bgcolor=#f0f0f0";
				
					   //if($ultimaDataVistoria == $chave["dtinclusao"])  $bgcolortr = "bgcolor=#ffffcc";
							
					   echo $bgcolortr;?>>
					<td>
						<img src="/imagens/alterar.gif" border="0" title="Editar" onclick="javascript:AtualizarVistoria('?modulo=principal/inserir_vistoria&acao=A', <?php echo $chave["supvid"]; ?>);"> 
						
						<?php
							//echo (verificaValidacaoVistoria($chave["supvid"]) &&(  ($chave["usucpf"] == $_SESSION['usucpf'] && $ultimaDataVistoria == $chave["dtinclusao"]) || ($db->testa_superuser() && $ultimaDataVistoria == $chave["dtinclusao"])) /*|| possuiPerfil(array(PERFIL_ADMINISTRADOR) )*/ && !$boBloqueioAba) ? sprintf( $stBotaoExcluirHabilitado, $chave['supvid'] ): $stBotaoExcluir;
							echo (verificaValidacaoVistoria($chave["supvid"]) &&(  ($chave["usucpf"] == $_SESSION['usucpf'] && $ultimaDataVistoria == $chave["dtvistoria"]) || ($db->testa_superuser() && $ultimaDataVistoria == $chave["dtvistoria"])) && !$boBloqueioAba) ? sprintf( $stBotaoExcluirHabilitado, $chave['supvid'] ): $stBotaoExcluir;
						?>
						
					</td>
					<td align="center">
						<? echo ((!empty($possuiFotos))? " <img src=\"/imagens/cam_foto.gif\" title=\"Foto(s) da Vistoria\"> " : " - ");  ?>
					</td>
					<td align="center">
						<? print($chave['aditivo']); ?>
					</td>
					<td align="center">
						<? print($chave['supvid']); ?>
					</td>
					<td align="center">
						<? print($i); ?>
					</td>
					<td>
						<? print($chave["dtvistoria"]); ?>
					</td>
					<td>
						<? print($chave["dtinclusao"]); ?>
					</td>
					<td>
						<?php if( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_ADMINISTRADOR) ) ){ ?>
							<img border="0" onclick='envia_email("<?php echo $chave["usucpf"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
						<?php } ?>
						<? print($chave["usunome"]); ?>
					</td>
					<td>
						<? print($chave["stodesc"]); ?>
					</td>
					<td>
						<?php if( !empty($chave["vistoriador"]) && ( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_ADMINISTRADOR) ) ) ){ ?>
							<img border="0" onclick='envia_email("<?php echo $chave["cpfvistoriador"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
						<?php } ?>
						<? !empty($chave["vistoriador"]) ? print($chave["vistoriador"]) : print('NÃO INFORMADO'); ?>
					</td>
					<td>
						<? print($chave["responsavel"]); ?>
					</td>
					<td>
						<?php
							$percentual = $chave["percentual"];
							$percentual = $percentual > 100.00 ? 100.00 : $percentual;
							print(number_format($percentual,2,',','.')); 
						?> %
					</td>
				</tr>
				<? $i++; }} ?>
				
			</table>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>
				<div style="float: left;">
					<?php
						$sql = "SELECT DISTINCT 
									stoid, max(supvdt) as supdtinclusao 
								FROM 
									obras.supervisao s
								inner join (SELECT max(supvdt) as dtinclusao 
								FROM 
									obras.supervisao 
								WHERE 
									supstatus = 'A' AND obrid = {$_SESSION['obra']['obrid']} ) a on a.dtinclusao = 	s.supdtinclusao
								
								WHERE 
									supstatus = 'A' AND obrid = {$_SESSION['obra']['obrid']} 
								GROUP BY 
									stoid, supstatus";
					
						$dado_situacao = $db->carregar($sql);
					?>
					<?php 
					//Recuperando Dados do Aditivo.
					$obAditivo = pegaObUltimoAditivo();
					$obAditivo = is_object($obAditivo) ? $obAditivo : new stdClass();
					$obAditivo->travlrfinalobra = pegaObMaiorVlrAditivo();	
					$traid 	   = $obAditivo->traid;
					$traseq    = $obAditivo->traseq;
					$arrParam  = array("traid" => $traid, "traseq" => $traseq);
					//Recupera o Valor Total das etapas do Cronograma Físico-Financeiro.
					$valorTotalCronograma = recuperaValorTotalCronograma($_SESSION['obra']['obrid'],$arrParam);
					//Igualando funcionalidade a da Página de Cronograma a pedido do Mario - 30/07/2012 10:32
					//$valorTotalCronograma = ($obAditivo->travlrfinalobra ? $obAditivo->travlrfinalobra : $_SESSION["obrcustocontrato"]);
					//Recupera o Valor Total do Contrato da Obra com ou sem Aditivo.
					$valorTotalContrato   = recuperaValorTotalContrato($_SESSION['obra']['obrid'], $arrParam);
					//Se o Valor Total do Cronograma Físico-Financeiro for divergente do Valor Total do Contrato da Obra com ou sem Aditivo,
					//é se a situação for diferente de Em Licitação - 5, Em Elaboração de Projetos - 4, Em Planejamento pelo Proponente - 99
					//não será possível inserir uma nova Vistoria.
					if(
						$valorTotalCronograma != $valorTotalContrato 
						&& $dobras->getStoId() != '4' 
						&& $dobras->getStoId() != '5' 
						&& $dobras->getStoId() != '9' 
						&& $dobras->getStoId() != '99'){
						//Desabilitando o botão para inserir uma novo Vistoria.
						$desabilitaBotao = 'disabled=\"disabled\"';
						//Mensagem que será apresentada quando os Valores do Cronograma e do Contrato forem divergentes.
						$mensagem = '&nbsp;&nbsp;&nbsp;<font style="color: red;"><b> Valor das Etapas divergente do valor do Contrato! </b></font>';
					}
					?> 
					<?php if( ($habilitado && $dado_situacao[0]['stoid'] != 3) || ( $dado_situacao[0]['stoid'] != 3 && !obraAditivoPossuiVistoria()) ){ ?>
						<input <?=$desabilitaBotao ?> style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Vistoria" onclick="window.location='?modulo=principal/inserir_vistoria&acao=A'" />
					<?php // era }else if( $dado_situacao[0]['stoid'] == 3 && possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_GESTORMEC, PERFIL_EMPRESA, PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE ) ) ){
						  }else if( $dado_situacao[0]['stoid'] == 3 && possuiPerfil( array( PERFIL_SUPERVISORMEC ) )  ){  
					?>
						<input <?=$desabilitaBotao;?> style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Vistoria" onclick="window.location='?modulo=principal/inserir_vistoria&acao=A'" />
					<?php }else if( possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_GESTORMEC, PERFIL_EMPRESA, PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE ) ) 
						  			&& !$boBloqueioAba ){ ?>
						<input <?=$desabilitaBotao;?> style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Vistoria" onclick="window.location='?modulo=principal/inserir_vistoria&acao=A'" />
					<?php }else{ ?>
						<input <?=$desabilitaBotao; ?><?=(($habilitado)? "" : "disabled=\"disabled\"" );?> style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Vistoria" onclick="window.location='?modulo=principal/inserir_vistoria&acao=A'"/>
					<?php } ?>
					<input <?=(($habilitado && ! $inclusaoEmpresaObraFinalizada )? "" : "disabled=\"disabled\"" );?> style="cursor: pointer;" type="button" name="gerar_formulario" value="Formulário de Vistoria" onclick="window.open('?modulo=principal/relatorio_supervisao_imp&acao=A','blank','height=450,width=550,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');"/>
					<?php echo $mensagem; ?>
				</div> 
			</td>
		</tr>
	</table>
</form>
<?php chkSituacaoObra(); ?>