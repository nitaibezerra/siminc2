<?php
if ( isset( $_REQUEST['buscar'] ) )
{
	include "geral_ppp_resultado.inc";
	exit();
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Relatório PPP", "" );
//monta_titulo_cte( $titulo_modulo );
?>
<script type="text/javascript">
	function abrepopupMunicipio(){
		janela = window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/combo_municipios.php','Municipios','width=400,height=400,scrollbars=1,top=200,left=480');
		janela.focus();
	}
	
	function abrepopupEscolas(){
		janela = window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/combo_escolas.php','Escolas','width=400,height=400,scrollbars=1,top=250,left=500');
		janela.focus();
	}
	
	function abrepopupCursos(){
		janela = window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/combo_cursos.php','Cursos','width=400,height=400,scrollbars=1,top=250,left=500');
		janela.focus();
	}
</script>
<script type="text/javascript"><!--

	function exibirRelatorio()
	{
		var formulario = document.formulario;
		
		// verifica se tem algum agrupador selecionado
		agrupador = document.getElementById( 'agrupador' );
		if ( !agrupador.options.length )
		{
			alert( 'Escolha ao menos um item para agrupar o resultado.' );
			return;
		}
		
		selectAllOptions( agrupador );
		selectAllOptions( document.getElementById( 'regcod' ) );
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'mescod' ) );
		selectAllOptions( document.getElementById( 'municipios' ) );
		selectAllOptions( document.getElementById( 'escolas' ) );
		selectAllOptions( document.getElementById( 'cursos' ) );
		selectAllOptions( document.getElementById( 'ssuid' ) );
		selectAllOptions( document.getElementById( 'frmid' ) );
		selectAllOptions( document.getElementById( 'tplid' ) );

		// submete formulario
		formulario.target = 'resultadoPPPGeral';
		var janela = window.open( '', 'resultadoPPPGeral', 'width=780,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
		formulario.submit();
		janela.focus();
	}

	function informarOfertaEPT(){
		var eptSim = document.getElementById('entdoferta_ept_sim');
		var eptNao = document.getElementById('entdoferta_ept_nao');
			
		if( eptSim.checked ){
			document.getElementById('statusOferta').style.display = '';
		}
		else{
			document.getElementById('statusOferta').style.display = 'none';
			document.getElementById('funcionamento').checked = false;
			document.getElementById('previsto').checked = false;
		}
	}

--></script>
<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
		<!-- AGRUPADOR ----------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Agrupadores
			</td>
			<td width="80%">
				<?php
$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <!--
                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                -->
                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;
					// inicia agrupador
					$agrupador = new Agrupador( 'formulario', $agrupadorHtml );
					$destino = array();
					$origem = array(
						'pais' => array(
							'codigo'    => 'pais',
							'descricao' => 'País'
						),
						'regiao' => array(
							'codigo'    => 'regiao',
							'descricao' => 'Região'
						),						
						'estado' => array(
							'codigo'    => 'estado',
							'descricao' => 'Unidade da Federação'
						),
						'mesorregiao' => array(
							'codigo'    => 'mesorregiao',
							'descricao' => 'Mesorregião'
						),
						'municipio' => array(
							'codigo'    => 'municipio',
							'descricao' => 'Municipio'
						),
						'escola' => array(
							'codigo'    => 'escola',
							'descricao' => 'Escola'
						),
						'curso'	=> array(
							'codigo' 	=>	'curso',
							'descricao'	=>	'Curso'
						)
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
					
				?>
			</td>
		</tr>
        <tr>
     	   <td>Filtros Territórios</td>
        </tr>		
		<!-- REGIAO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Regiões
			</td>
			<td>
				<?php
				$sqlComboRegiao = "
					select
						regcod as codigo,
						regdescricao as descricao
					from territorios.regiao
					order by
						regdescricao
				";
				combo_popup( "regcod", $sqlComboRegiao, "Regiões", "192x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- ESTADO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Unidades da Federação
			</td>
			<td>
				<?php
				$sqlComboEstado = "
					select
						estuf as codigo,
						estdescricao as descricao
					from territorios.estado
					order by
						estdescricao
				";
				combo_popup( "estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
        <tr>
        <td class="SubTituloDireita" valign="top" width="20%">Mesoregião</td>
        <td>
        <?php
                $sqlComboMesoregiao = "
                   select
                       estuf || ' - ' || mesdsc as descricao,
                       mescod as codigo
                    from territorios.mesoregiao
                    order by
                        estuf
                ";
                combo_popup( "mescod", $sqlComboMesoregiao, "Mesoregião", "400x400", 0, array(), "", "S", false, false, 5, 400 );
        ?>
        </td>
        </tr>
            <tr>
        <td class="SubTituloDireita" valign="top">Município</td>
        <td>
	        <select multiple="multiple" size="5" name="municipios[]" 
	        id="municipios"  
	        ondblclick="abrepopupMunicipio();"  
	        class="CampoEstilo" style="width:400px;" >
	        <option value="">Duplo clique para selecionar da lista</option>
	        </select>
        </td>
    </tr>
    
    <!-- LOCALIZACAO ------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Localização
			</td>
			<td>
				<?php
				$sqlComboLocalizacao = "
					select
						tplid as codigo,
						tpldesc as descricao
					from entidade.tipolocalizacao
					order by tpldesc
				";
				combo_popup( "tplid", $sqlComboLocalizacao, "Localização", "210x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
    
    <tr>
        <td>Filtro Escolas</td>
    </tr>	
    <tr>
        <td class="SubTituloDireita" valign="top">Escolas</td>
        <td>
        <?php
        /*
                $sqlComboEscolas = "
                    select mundescricao as nome,
                            estuf as siglaestado
                    from territorios.municipio
                    order by estuf
                ";
		*/
                //combo_popup( "mundescricao", $sqlComboEscolas, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400 );
        ?>
         <select multiple="multiple" size="5" name="escolas[]" 
        id="escolas"  
        ondblclick="abrepopupEscolas();"  
        class="CampoEstilo" style="width:400px;" >
        <option value="">Duplo clique para selecionar da lista</option>
        </select>
        </td>
    </tr>   
        <tr>
        <td>Filtro Cursos</td>
    </tr>	
    <tr>
        <td class="SubTituloDireita" valign="top">Cursos</td>
        <td>
         <select multiple="multiple" size="5" name="cursos[]" 
        id="cursos"  
        ondblclick="abrepopupCursos();"  
        class="CampoEstilo" style="width:400px;" >
        <option value="">Duplo clique para selecionar da lista</option>
        </select>
        </td>
    </tr>  
    <tr>
		<td>Subação</td>
    </tr>
	<!-- Status Subação -------------------------------------------------------- -->
	<!-- Ano de Referência -------------------------------------------------------- -->
	<tr>
		<td class="SubTituloDireita" valign="top">
			Ano de Referência
		</td>
		<td>
			<select id="anoReferencia" name="anoReferencia">
				<option value="0">Todos</option>
				<?php for($x=2008;$x<=date('Y');$x++): ?>
					<option value="<?php echo $x ?>"><?php echo $x ?></option>
				<?php endfor; ?>
			</select>
		</td>
	</tr>    
	<tr>
		<td class="SubTituloDireita" valign="top">
			Status da Subação
		</td>
		<td>
			<?php
			$sqlComboStatusSubacao = "
				select 
					ssuid as codigo, 
					ssudescricao as descricao 
				from cte.statussubacao
				WHERE ssutipostatus = 'E'
				order by ssudescricao
			";
			combo_popup( "ssuid", $sqlComboStatusSubacao, "Todos status", "192x400", 0, array(), "", "S", false, false, 5, 400 );
			?>
		</td>
	</tr>    
	<!-- Forma de Execução -------------------------------------------------------- -->
	<tr>
		<td class="SubTituloDireita" valign="top">
			Forma de Execução
		</td>
		<td>
			<?php
			$sqlComboFormaExecucao = "
				select 
					frmid as codigo,
					frmdsc as descricao
				from cte.formaexecucao
				where frmbrasilpro = true
				and frmtipo = 'E'
			";
			combo_popup( "frmid", $sqlComboFormaExecucao, "Todas as Formas de Execução", "192x400", 0, array(), "", "S", false, false, 5, 400 );
			?>
		</td>
	</tr>    
    
    <tr>
		<td>Filtros Diversos</td>
    </tr>
    
    <tr>
        <td class="SubTituloDireita" valign="top">Possui formação EPT</td>
        <td>
        	<input onclick="informarOfertaEPT();" type="radio" name="filtroept" id="entdoferta_ept_sim" value="sim"> Sim &nbsp;&nbsp;&nbsp;&nbsp; 
        	<input onclick="informarOfertaEPT();" type="radio" name="filtroept" id="entdoferta_ept_nao" value="nao"> Não &nbsp;&nbsp;&nbsp;&nbsp; 
        	<input onclick="informarOfertaEPT();" type="radio" name="filtroept" id="entdoferta_ept_todos" value="todos" checked> Todos
        </td>
    </tr>
    <tr id="statusOferta" style="display: none;">
        <td class="SubTituloDireita" valign="top">Status Formação EPT</td>
        <td>
        	<input type="radio" name="statusOferta" id="funcionamento" value="F"> Em Funcionamento &nbsp;&nbsp;&nbsp;&nbsp; 
        	<input type="radio" name="statusOferta" id="previsto" value="P"> Previsto &nbsp;&nbsp;&nbsp;&nbsp; 
        	<input type="radio" name="statusOferta" id="statusTodos" value="todos" checked> Todos
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" valign="top">PPP preenchido</td>
        <td>
        	<input type="radio" name="filtroppp" value="sim"> Sim &nbsp;&nbsp;&nbsp;&nbsp; <input type="radio" name="filtroppp" value="nao"> Não &nbsp;&nbsp;&nbsp;&nbsp; <input type="radio" name="filtroppp" value="todos" checked> Todos
        </td>
    </tr>
    
    <tr>
		<td class="SubTituloDireita" valign="top">
			&nbsp;
		</td>
		<td class="SubTituloDireita" style="text-align:left;">
			<input
				type="button"
				name="filtrar"
				value="Visualizar"
				onclick="exibirRelatorio();"
			/>
		</td>
	</tr>
</table>		