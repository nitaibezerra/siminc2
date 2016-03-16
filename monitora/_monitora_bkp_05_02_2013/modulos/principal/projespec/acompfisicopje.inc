<?
	/*
	sistema simec
	setor responsável: spo-mec
	desenvolvedor: equipe consultores simec
	Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
	Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), 
	Cristiano Cabral (cristiano.cabral@gmail.com), Thiago Mata thiago.henrique.mata@gmail.com
	módulo:acompfisicopje.inc
	finalidade: permitir o acompanhamento físico do projeto
	*/

	$modulo=$_REQUEST['modulo'];
	$acao = $_REQUEST['acao'];
	
	include APPRAIZ."includes/cabecalho.inc";
	

if (! $_SESSION['pjeid']) 
{
	?>
	<script>alert('Escolha primeiro o Projeto Especial!');
	history.back();</script>
	<?
	$db->close();
	exit();
}
else $pjeid=$_SESSION['pjeid'];	
	

	function recarregar_arvore()
	{
		$location = $_SESSION['sisdiretorio'] . ".php?modulo=" . $_REQUEST['modulo'] . "&acao=" . $_REQUEST['acao'];
		?>
			<script type="text/javascript">
			  	location.href = "<?= $location ?>";
			</script>
		<?php
		exit();
	}
	
	function carrega_filhos_do_registro( $objRegistro = null , $intPjeId )
	{
		global $db;
		
		$strSql = " SELECT
						p.ptoid									AS id,
						p.ptoorigemppa							AS origem,
						p.ptotipo								AS tipo,
						p.ptocod								AS codigo,
						p.ptoordem   							AS ordem ,
						p.ptoid_antecessor						AS antecessora ,
						p.usucpf								AS cpf_dono,
						p.ptonivel								AS nivel,
						p.ptoprevistoexercicio					AS meta, 
						p.ptotipo								AS tipo ,
						p.ptodsc								AS nome,
						p.ptodescricao							AS descricao,
						to_char( p.ptodata_ini , 'dd/mm/yyyy' )	AS data_inicio, 
						to_char( p.ptodata_fim , 'dd/mm/yyyy' )	AS data_termino,
						p.procod								AS produto_id,
						produto.prodsc							AS produto_nome,
						unidademedida.unmdsc					AS unidade_medida
					FROM 
						monitora.planotrabalho p
					LEFT JOIN
						public.unidademedida unidademedida
					USING
						( unmcod )
					LEFT JOIN
						public.produto as produto
					USING
						( procod )
					WHERE
						pjeid = " . $intPjeId . "
					AND
						p.ptoid_pai " . ( $objRegistro[ 'id' ] ? ' = ' . $objRegistro[ 'id' ] : 'is null' ) . "
					AND
						p.ptostatus = 'A'
					ORDER BY
						p.ptoordemacao	
					";
		
		$arrFilhos = $db->carregar( $strSql );
		$arrFilhosDoRegistro = array();
		
		if( $arrFilhos )
		{
			foreach( $arrFilhos as $arrRegistro )
			{
				$strSql =	"
							SELECT
								exprealizado 	AS realizado,
								expobs			AS observacao ,
								expdata			As ultima_data 
							FROM
								monitora.execucaopto
							WHERE
								ptoid = " . $arrRegistro[ 'id'] . "  
							AND
								pjeid = " . $intPjeId . "
							order by expdata, expid desc LIMIT 1
							";
				
				$arrRealizacao = $db->carregar( $strSql );
				if( $arrRealizacao )
				{
					$objRealizacao = array_pop( $arrRealizacao );
				}
				else
				{
					$objRealizacao = array();
				}
				
				$sqlStatus =	"select
									t.tpsdsc as status, 
									t.tpscor as cor 
								from 
									public.tiposituacao t 
								inner 
									join monitora.execucaopto e 
								on
									e.tpscod = t.tpscod 
								where
									e.ptoid = ".$arrRegistro[ 'id']." 
								order by
									e.expdata, e.expid 
								desc limit 1";

				$rsStatus = @$db->recuperar( $sqlStatus );
				$strStatus = $rsStatus[ "status" ] ? $rsStatus[ "status" ] : "S/ avaliação";
				$strCorStatus = $rsStatus[ "cor" ] ? $rsStatus[ "cor" ] : "";
				
				$objRegistroFilho = array();
				$objRegistroFilho[ 'id' ] 					= $arrRegistro[ 'id'];  
				$objRegistroFilho[ 'origem' ] 				= ( $arrRegistro[ 'origem'] == 't' );  
				$objRegistroFilho[ 'tipo' ] 				= $arrRegistro[ 'tipo'];  
				$objRegistroFilho[ 'codigo' ] 				= $arrRegistro[ 'codigo'];  
				$objRegistroFilho[ 'ordem' ] 				= $arrRegistro[ 'ordem'];  
				$objRegistroFilho[ 'antecessora' ]		 	= $arrRegistro[ 'antecessora'];  
				$objRegistroFilho[ 'cpf_dono' ] 			= $arrRegistro[ 'cpf_dono'];  
				$objRegistroFilho[ 'antecessora' ] 			= $arrRegistro[ 'antecessora'];  
				$objRegistroFilho[ 'cpf_dono' ] 			= $arrRegistro[ 'cpf_dono'];  
				$objRegistroFilho[ 'nivel' ] 				= $arrRegistro[ 'nivel'];  
				$objRegistroFilho[ 'previsto_exercicio' ] 	= $arrRegistro[ 'previsto_exercicio'];  
				$objRegistroFilho[ 'tipo' ] 				= $arrRegistro[ 'tipo'];  
				$objRegistroFilho[ 'nome' ] 				= $arrRegistro[ 'nome'];  
				$objRegistroFilho[ 'descricao' ] 			= $arrRegistro[ 'descricao'];  
				$objRegistroFilho[ 'data_inicio' ] 			= $arrRegistro[ 'data_inicio'];  
				$objRegistroFilho[ 'data_termino' ] 		= $arrRegistro[ 'data_termino'];  
				$objRegistroFilho[ 'produto_id' ] 			= $arrRegistro[ 'produto_id'];  
				$objRegistroFilho[ 'produto_nome' ] 		= $arrRegistro[ 'produto_nome'];  
				$objRegistroFilho[ 'meta' ] 				= $arrRegistro[ 'meta'];
				$objRegistroFilho[ 'unidade_medida' ] 		= $arrRegistro[ 'unidade_medida'];
				$objRegistroFilho[ 'unidade_medida' ] 		= $arrRegistro[ 'unidade_medida'];
				$objRegistroFilho[ 'realizado' ]			= @$objRealizacao[ 'realizado' ];
				$objRegistroFilho[ 'observacao' ]			= @$objRealizacao[ 'observacao' ];
				$objRegistroFilho[ 'ultima_data' ]			= @$objRealizacao[ 'ultima_data' ];
				$objRegistroFilho[ 'status' ]				= $strStatus;
				$objRegistroFilho[ 'corstatus' ]			= $strCorStatus;
				$objRegistroFilho[ 'pai_id' ]				= $objRegistro[ 'id' ];
				$objRegistroFilho[ 'pje_id' ]				= $intPjeId;
				
				$arrFilhosDoRegistro[] = $objRegistroFilho;
			}
		}
				
		return $arrFilhosDoRegistro;
	}
	
	function mostra_registro( $objRegistro , $strOrdemRelativa, $boolVisibilidade, $cor )
	{
		?>
			<!-- registro -->
			<tr id="registro<?= @$objRegistro['id'] ?>" bgcolor="<?= $cor ?>" style="padding-left: 5px; display:<?= ( $boolVisibilidade ) ? 'table-row' : 'none' ?>" parent="<?= $objRegistro[ 'pai_id' ] ?>" onmouseout="this.bgColor='<?= $cor ?>';" onmouseover="this.bgColor='#ffffcc';">
				<td>
					<?= $objRegistro[ 'ordem' ] ?>
				</td>
				<td align="right">
					<img border="0" src="../imagens/alterar.gif" title="Acompanhar a Tarefa." onclick="editartarefa('<?=$objRegistro[ 'id' ] ?>')"/>
					<? if ( @$objRegistro['qtd_filhos' ] > 0 ) : ?>
						<img border="0"  src="../imagens/lupa_grafico.gif" onclick="exibe_grafico( 'Projet' , <?= $objRegistro[ 'pje_id' ] ?> , <?= $objRegistro[ 'id' ] ?> )" />
					<? else: ?>
						<img style="visibility: hidden" border="0"  src="../imagens/lupa_grafico.gif" onclick="exibe_grafico( 'Acao' , <?= $objRegistro[ 'pjeid' ] ?> , <?= $objRegistro[ 'id' ] ?> )" />
					<? endif ?>
				</td>
				<td align="left" style="padding-left: <?= 5 + ( 30 * ( @$objRegistro[ 'nivel' ] - 1 ) ) ?>px">
					<? if ( @$objRegistro['qtd_filhos' ] > 0 ) : ?>
						<img 
							id="img<?=  @$objRegistro['id'] ?>" atividade="<?= @$objRegistro['id'] ?>" 
							onclick="exibirOcultarAtividadesFilhas( <?=  @$objRegistro['id']  ?>, this, true );" 
							pai="imgTarefa<?=  @$objRegistro['pai_id']  ?>" 
							src="<?= !$objRegistro[ 'ultimonivel' ] ? '../imagens/menos.gif' : '../imagens/mais.gif' ?>" />
					<? else: ?>
						<img
							style="visibility: hidden" 
							src="<?= !$objRegistro[ 'ultimonivel' ] ? '../imagens/menos.gif' : '../imagens/mais.gif' ?>" />
					<? endif ?>				
					&nbsp;					
					<? if ( @$objRegistro['qtd_filhos' ] <= 0 ) : ?>
						<?= $strOrdemRelativa ?>
						-
						<?= $objRegistro[ 'nome' ] ?>
					<? else: ?>
						<b>
							<?= $strOrdemRelativa ?>
							-
							<?= $objRegistro[ 'nome' ] ?>
						<b/>
					<? endif ?>
				</td>
				<!--<td align="left">
					<?= $objRegistro[ 'descricao' ] ?>
				</td>-->
				<td align="left">
					<?= $objRegistro[ 'produto_nome' ] ?>
				</td>
				<td class="coluna_data" align="right">
					<?= $objRegistro[ 'data_inicio' ] ?>
				</td>
				<td class="coluna_data" align="right">
					<?= $objRegistro[ 'data_termino' ] ?>
				</td>
				<td align="right">
					<?= (integer) $objRegistro[ 'realizado' ] ?> <?//= $objRegistro[ 'unidade_medida' ] ?> de <?= $objRegistro[ 'meta' ] ?> <?= $objRegistro[ 'unidade_medida' ] ?> <br>
					<?=monta_barrap($objRegistro[ 'realizado' ], $objRegistro[ 'meta' ]);?>
				</td>
				<td align="left" style="color:<?=$objRegistro[ 'corstatus' ]?>;">
					<b><?= $objRegistro[ 'status' ] ?></b>
				</td>
			</tr>
			<!-- fim do registro -->
		<?
	}

	function monta_arvore_cabecalho( $arrRegistros , $intPjeId , $intLimiteProfundidade )
	{
		?>
			<table border="0" id="atividades" width="95%" cellspacing="1" cellpadding="2" border="0" align="center" class="listagem" style="color: rgb(51, 51, 51);">
				<thead>
					<tr>
						<th align="right">
						</th>
						<th align="right">
							Ações
						</th>
						<th align="left">
							Nome
						</th>
						<!--<th align="left">
							Descrição
						</th>-->
						<th align="left">
							Produto
						</th>
						<th align="right">
							Data de Inicio
						</th>
						<th align="right">
							Data de Término
						</th>
						<th align="center">
							Execução
						</th>
						<th align="left">
							Status
						</th>
					</tr>
				</thead>
				<tbody>
					<?
						monta_arvore_dos_registros( $arrRegistros, 0 , $intLimiteProfundidade , '' , $intPjeId );
					?>
				</tbody>
			</table>
		<?
	}
	function monta_arvore_dos_registros( $arrRegistros , $intNivel, $intProfundidadeMaxima , $strOrdemRelativa = '' , $intPjeId )
	{
		$intNivel++;
		foreach( $arrRegistros as $intPosicao => $objRegistro )
		{
			if( $strOrdemRelativa == '' )
			{
				$strOrdemRelativaFilha = ( $intPosicao + 1 );
			}
			else
			{
				$strOrdemRelativaFilha =  $strOrdemRelativa . '.' . ( $intPosicao + 1 );
			}
			
			$boolVisibilidade = $intNivel <= $intProfundidadeMaxima;
			$arrFilhosDoRegistro = carrega_filhos_do_registro( $objRegistro , $intPjeId );
			$objRegistro[ 'qtd_filhos' ] = sizeof( $arrFilhosDoRegistro );
			$objRegistro[ 'nivel' ] = $intNivel;
			$objRegistro[ 'ultimonivel' ] = ( $intNivel == $intProfundidadeMaxima );
			$cor = ( $cor == '#f7f7f7' ) ? '#ffffff' : '#f7f7f7';
			mostra_registro( $objRegistro , $strOrdemRelativaFilha , $boolVisibilidade, $cor );
			monta_arvore_dos_registros( $arrFilhosDoRegistro , $intNivel , $intProfundidadeMaxima , $strOrdemRelativaFilha , $intPjeId );
		}
	}
	
	function mostrar_registros_da_acao( $intPjeId )
	{
		$arrRegistrosRaizDaAcao = carrega_filhos_do_registro( null , $intPjeId );
		// coloque aqui ate que nivel a arvore devera vir aberta //
		$intProfundidadeMaximaAberta = 1;
		monta_arvore_cabecalho( $arrRegistrosRaizDaAcao , $intPjeId , $intProfundidadeMaximaAberta );
	}
	
	function monta_barrap($realizado, $meta){
		
		if ($meta>0) $porcentorealizado = number_format($realizado * 100 / $meta, 0, '.', '');
		else $porcentorealizado= number_format(100, 0, '.', '');
		if ($porcentorealizado > 100 ) {$porcentorealizado2 = 100;$cor_barra = "#33cc00";} else {$cor_barra = "#0033cc";$porcentorealizado2=$porcentorealizado;}
		?><label style="border: 1px solid #000000; font-size: 8px; border-top: 1px solid #c0c0c0; border-right: 1px solid #c0c0c0; border-left: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0;background-color:#ffffff;" title="Realizado <?=number_format($porcentorealizado, 0, '.', '')?>%"><span style="background-color:<?=$cor_barra;?>;font-size: 8px;"><?for ($k=0; $k<$porcentorealizado2/2;$k++) print "&nbsp;";?></span><?for ($k=0; $k<(100-$porcentorealizado2)/2;$k++) print "&nbsp;";?></label><?
		//if ($porcentorealizado < 100) print number_format($porcentorealizado, 0, '.', ''); else print '100';
	}

?>

<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Acompanhamento do Plano de Trabalho';
monta_titulo($titulo_modulo,'');
?>
<style>
.coluna_data
		{
			color:#4488CC;
			
		}
</style>
<form name="formulario" method="post">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <?$db->cabecalho_projeto($_SESSION['pjeid']);?>
	<tr>
		<td align='right' class="SubTituloDireita" colspan="2" style="height:1px;padding:0px;"></td>
	</tr>
</table>
</form>

<?
		mostrar_registros_da_acao( $_SESSION['pjeid'] );
?>


<script>

function mostra_equipe()
{
	e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
	window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
}
  

function editartarefa(cod)
{
	 e = "monitora.php?modulo=principal/projespec/editapje&acao=A&ptoid="+cod;
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=600'");
}

function filtra()
{
	document.formulario.submit();
}
</script>
<style>

		.coluna_data
		{
			color:#4488CC;
			
		}
</style>

<script type="text/javascript">
		
		function popup_arquivo( acao, nome, width, height )
		{
			window.open( '../geral/popup_arquivoupload.php?acao=' + acao + '&nome=' + nome, '', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
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
	
function exibe_grafico( strContainer, intContainerId , intTarefaId_ , intNivel_ )
{
	intTarefaId_ = ( forceInt( intTarefaId_ ) );
//	alert( intTarefaId_ );
	if( intNivel_ == undefined  )
	{
/*
		var objSelect = document.getElementById( "profunidadeNivel" );
		var intNivel_ = objSelect.value;
*/
		var intNivel_ = 10; 
	}
	
	var strLink = "../geral/gantt.php?container=" + strContainer + "&containerId=" + intContainerId + "&nivel=" + intNivel_ +"&tarefa=" + intTarefaId_;
    window.open( 
    	strLink ,
    	"janela",
    	"menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes" );
}

	
</script>
