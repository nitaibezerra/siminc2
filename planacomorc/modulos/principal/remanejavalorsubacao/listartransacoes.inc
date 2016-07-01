<?php
/**
 * Listagem de transações de remanjemanto de subações.
 * $Id: listartransacoes.inc 102359 2015-09-11 18:26:07Z maykelbraz $
 */

/**
 *
 */
include_once '_funcoesremanejavalorsubacao.php';

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";
?>
<script src="./js/planacomorc.js"></script>
<script>
    function verTransacao(rmpid) {
        location.href = '/planacomorc/planacomorc.php?modulo=principal/remanejavalorsubacao/remaneja&acao=A&rmsid=' + rmpid;
    }

    function excluirTransacao(rmsid) {
        var r = confirm("Você realmente deseja excluir essa Transação ?");
        var x = '';
        if (r === true){
            var url = '/planacomorc/planacomorc.php?modulo=principal/remanejavalorsubacao/listartransacoes&acao=A';

            $.ajax({
              type: "POST",
              url: url,
              data: { requisicao: "excluiTransacaoRms", rmsid: rmsid }
            }).done(function( msg ) {
                var str = '';
                if(msg === 'true'){
                    str = 'Transação excluída com sucesso.';
                    alert(str);
                    window.location = window.location;
                }else{
                    str = 'A transação não pode ser excluída.';
                    if(msg != 'false'){
                        str += '\n'+msg;
                    }
                    alert(str);
                }
            });
        }
    }
</script>

<?php
/* Seleciona o tipo de transação a ser filtrado, default 'S' (Solicitações) */
$tipoTransacao = 'S';
if ($_REQUEST['tipoTransacao']) {
    $tipoTransacao = $_REQUEST['tipoTransacao'];
}
if ($_REQUEST['buscalivre']) {
    $buscaLivreTmp = removeAcentos(str_replace("-"," ",$_REQUEST['buscalivre']));
    $whereBuscaLivre = "AND (";
    $whereBuscaLivre .= "public.removeacento(rms.po) ILIKE '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(rms.podsc) ILIKE '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(ptr.ptres) ILIKE '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(rms.funcprogramatica) ILIKE '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(usu.usunome) ILIKE '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(uni.unidsc) ilike '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= ")";
}
if($tipoTransacao == 'S'){
    $colunaTransacaoEfetivada = ",CASE WHEN rms.rmpstprocessado THEN 'SIM' ELSE 'NÃO' END AS processado";
}
$whereRestricao = retornaFiltroEfetivarRemanejamentoSubacoesUoUg();

$btn_excluir = '';


// CASE WHEN NOT rms.rmpstprocessado
// THEN '<a style="cursor:pointer" onclick="verTransacao(''' || rms.rmsid || ''');">'
// 		|| '<img src="/imagens/alterar.gif" border="0" title="Alterar"></a>&nbsp;'
// 		{$btn_excluir}
// 		ELSE '-' END AS acao,

$sql_lista = <<<DML
	SELECT
		rms.rmsid,
       	ptr.ptres,
       	rms.po || ' - ' || rms.podsc,
       	rms.funcprogramatica,
       	uni.unidsc as unidade,
       	TO_CHAR(rms.datatransacao, 'dd/mm/yyyy') AS datatransacao,
       	usu.usunome,
       	rms.rmpsaldosubtraido as subtraido,
       	rms.rmpsaldoadicionado as adicionado,
       	CASE WHEN NOT rms.rmpstprocessado THEN 1 ELSE 0 END AS permissao
       	{$colunaTransacaoEfetivada}
  	FROM planacomorc.remanejamentosubacao rms
    INNER JOIN monitora.ptres ptr USING(ptrid)
    LEFT JOIN public.unidade uni USING(unicod)
    INNER JOIN seguranca.usuario usu USING(usucpf)
  	WHERE rms.tipotransacao = '{$tipoTransacao}'
        {$whereBuscaLivre}{$whereRestricao}
  	ORDER BY rms.rmpstprocessado,rms.datatransacao DESC
DML;

$cabecalho = array(
	"PTRES",
    "PO",
    "Funcional Programática",
    "Unidade",
    "Data da Operação",
    "Usuário",
    "Total Subtraído (R$)",
    "Total Adicionado (R$)"
);
if($tipoTransacao == 'S'){
    $cabecalho[]="Processada";
}
?>
<html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
    </head>
    <body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
	    <section class="col-md-12">
			<ol class="breadcrumb">
				<li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
				<li class="">Subações <?=$_SESSION['exercicio'];?></li>
				<li class="active">Listar Transações de Remanejamento de Subação (por PTRES)</li>
			</ol>
		</section>
		<section class="col-md-12">
			<section class="well">
		        <form class="form-horizontal" action="" method="post" name="formulario">
		            <section class="form-group">
		            	<label class="control-label col-md-3">
		            		Pesquisar por:
		            	</label>
		            	<section class="col-md-9">
		            		<label>
		            			<input type="radio" name="tipoTransacao" value="S" <?php if ($tipoTransacao == 'S') echo 'checked=checked'; ?> /> Para Aprovação
		            		</label>
		            		<label>
		                    	<input type="radio" name="tipoTransacao" value="E" <?php if ($tipoTransacao == 'E') echo 'checked=checked'; ?> /> Aprovado
		                    </label>
		            	</section>
		            </section>
		           	<section class="form-group">
		           		<label class="control-label col-md-3" for="busca">Buscar:</label>
		           		<section class="col-md-6">
		           			<?php //echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, ''); ?>
		           			<?php inputTexto('buscalivre', $valor, 'busca', 150,false);?>
		           		</section>
		           	</section>
		           	<section class="form-group">
		           		<section class="col-md-9 col-md-offset-3">
		           			<input class="btn btn-primary" type="submit" name="botao" value="Pesquisar"/>
		           		</section>
		           	</section>
		        </form>
	        </section>
		</section>
		<section class="col-md-12">
        <?php
        	$listagem = new Simec_Listagem();
        	$listagem->setQuery($sql_lista);
        	$listagem->esconderColuna('permissao');
        	$listagem->setAcoes(array('edit'=>'verTransacao'))->turnOnPesquisator();
        	$listagem->setAcaoComoCondicional('edit', array(array('campo' => 'permissao', 'valor' => 0, 'op' => 'diferente')));
        	if(in_array(PFL_SUPERUSUARIO, pegaPerfilGeral())){//955
				$listagem->addAcao('delete', 'excluirTransacao');
        		$listagem->setAcaoComoCondicional('delete', array(array('campo' => 'permissao', 'valor' => 0, 'op' => 'diferente')));
        	}
        	$listagem->addCallbackDeCampo(array('subtraido','adicionado'), 'mascaraMoeda');
        	$listagem->setCabecalho($cabecalho);
        	$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        ?>
		</section>
    </body>
</html>
