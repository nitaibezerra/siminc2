<?php
include_once( APPRAIZ . "cte/classes/SubacaoIndicador.class.inc" );
include_once( APPRAIZ . "cte/classes/ComposicaoSubacao.class.inc" );
include_once( APPRAIZ . "www/includes/webservice/cpf.php" );
include_once( APPRAIZ . "includes/classes/modelo/public/TipoFormacao.class.inc" );
include_once( APPRAIZ . "includes/classes/modelo/public/TipoVinculoProfissional.class.inc" );
include_once( APPRAIZ . "cte/classes/ComposicaoPessoa.class.inc" );
//include_once( APPRAIZ . "includes/classes/modelo/entidade/Entidade.class.inc" );
include_once( APPRAIZ . "includes/classes/FormacaoEntidade.class.inc" );
include_once( APPRAIZ . "includes/classes/VinculoProfissionalEntidade.class.inc" );
include_once( APPRAIZ . 'includes/classes/entidades.class.inc');

//Aditivo //
$sbaidPai 	= $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; 
$anoAditivo = $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; 
$aditivo 	= $_REQUEST['ad'] ? $_REQUEST['ad'] : 0;

//Fim Aditivo //

$obSubacao = new SubacaoIndicador( $_REQUEST['sbaid'] );
$boExecucaoTecnica = cte_possuiFormaExecucaoTecnica( $obSubacao->frmid );

cte_verificaSessao();
$dadosSalvos                         = false;

$undid = $_REQUEST["undid"];
$arUnidades = cte_recuperarArUnidadesExigemCPF();
					 
$stCampos = " tfoid, tfodsc ";
$obTipoFormacao = new TipoFormacao();
$coTipoFormacao = $obTipoFormacao->recuperarTodos( $stCampos, array( "tfostatus = 'A'" ) );

$stCampos = " tvpid, tvpdsc ";
$obTipoVinculoProfissional = new TipoVinculoProfissional();
$coTipoVinculoProfissional = $obTipoVinculoProfissional->recuperarTodos( $stCampos, array( "tvpstatus = 'A'" ) );
					 
if (array_key_exists('excluir', $_REQUEST)) {
    $db->executar('DELETE FROM cte.composicaosubacao WHERE cosid = ' . (integer) $_REQUEST['excluir']);
    $db->commit();
    die('null');
}

if( $_REQUEST['req'] == 'removerItemComposicao' && ($_POST['cosid'] || $_POST['cspid']) ){
	if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica ){
	        $obComposicaoPessoa = new ComposicaoPessoa();
	        $obComposicaoPessoa->excluir($_POST['cspid']);
	        $obComposicaoPessoa->commit();
			exit;
    } else {
        $sql = ' 
        		DELETE FROM cte.escolacomposicaosubacao
        		WHERE cosid = ' . (integer) $_REQUEST['cosid'] . ';

        		DELETE FROM cte.composicaosubacao
        		WHERE cosid = ' . (integer) $_REQUEST['cosid'];

        $db->executar($sql);
        $db->commit();
    }
        die();
}

if ($_REQUEST['inserirItensLaboratorio']) {
    $sql ='
    SELECT
        il.*,
        udl.unddid,
        udl.undddsc
    FROM
        cte.laboratorio l JOIN cte.itenslaboratorio il USING(labid)
        JOIN cte.unidademedidadetalhamento udl USING(unddid) 
    WHERE
        l.undid = (SELECT undid FROM cte.subacaoindicador WHERE sbaid = ' .$_REQUEST['sbaid']. ')';

    $res   = $db->carregar($sql);
    $itens = array();

    while (list($i, $lab) = each($res)) {
        $itens[] = '{\'cosid\':\'\','
                 . ' \'cosdsc\':\''   . addslashes(str_replace(array("\n", "\r", "\r\n"), ' ', $lab['ilbdesc'])) . '\','
                 . ' \'unddid\':'     . $lab['unddid']              . ','
                 . ' \'cosano\':'     . $_REQUEST['cosano']         . ','
                 . ' \'modificado\':true,'
                 . ' \'cosqtd\':'     . $lab['ilbqtditens']         . ','
                 . ' \'cosvlruni\':\''. $lab['ilbvlrunitario']      . '\'}';
    }

    header('content-type: application/json;charset=iso-8859-1');
    echo 'var itensLaboratorio=[' , implode(',', $itens) , '];';
    die();
}

if (array_key_exists('btnGravar', $_REQUEST) && (is_array($_REQUEST['cosid']) || $_REQUEST["sbaid"]) ){
	if( $_REQUEST["boCPF"] ){
		
		$_REQUEST["arcos"] = ($_REQUEST["arcos"]) ? $_REQUEST["arcos"] : array() ;
		
		foreach( $_REQUEST["arcos"] as $count ){
			
			if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica ){
				
				if(!$_POST["coscpf_$count"]){
					$db->rollback();
					echo "<script>
				        	alert('O campo CPF é obrigatório.');
							history.back(-1);
						  </script>";
					die;
				}
				
				if(!$_POST["cosnome_$count"]){
					$db->rollback();
					echo "<script>
				        	alert('O campo Nome é obrigatório.');
							history.back(-1);
						  </script>";
					die;
				}
				
				if(!$_POST["comboVinculoProfissional_$count"]){
					$db->rollback();
					echo "<script>
				        	alert('O campo Vínculo é obrigatório.');
							history.back(-1);
						  </script>";
					die;
				}
				
				if(!$_POST["comboFormacao_$count"]){
					$db->rollback();
					echo "<script>
				        	alert('O campo Formação é obrigatório.');
							history.back(-1);
						  </script>";
					die;
				}
				
				$entid = $cspid = $foeid = $vpeid = "";
				$cpf = ereg_replace("[^0-9]", "", $_POST["coscpf_$count"] );

				$sql = "select entid from entidade.entidade where entnumcpfcnpj = '".$cpf."'";
				$entid = $db->pegaUm($sql);					

				$sql = "select entnome from entidade.entidade where entnumcpfcnpj = '".$cpf."'";
				$entnome = $db->pegaUm($sql);
				
				if(!$entid){ # se não tem cpf na base, cadastra
					$arDados = array('entnumcpfcnpj' => $cpf, 'entnome' => $_POST["cosnome_$count"] );
					$obEntidade = new Entidades();
					$obEntidade->carregarEntidade($arDados);
					$obEntidade->salvar();
					$entid = $obEntidade->getEntid();
				}
				
				if($entid){
					
					if($entnome == ''){						
						$arDados = array('entid' => $entid, 'entnumcpfcnpj' => $cpf, 'entnome' => $_POST["cosnome_$count"] );
						$obEntidade = new Entidades();
						$obEntidade->carregarEntidade($arDados);
						$obEntidade->salvar();
					}
					
					$sql = "select cspid from cte.composicaopessoa where entid = $entid and sbaid =  ".$_REQUEST["sbaid"] ." and cspano = ". $_REQUEST["cosano"];
					$cspid = $db->pegaUm($sql);
				}
				$obComposicaoPessoa = new ComposicaoPessoa($cspid);
				$obComposicaoPessoa->entid = $entid;
				$obComposicaoPessoa->sbaid = $_REQUEST["sbaid"];
				$obComposicaoPessoa->cspano = ($_REQUEST["cosano"]) ? $_REQUEST["cosano"] : date("Y");
				$obComposicaoPessoa->salvar();

				$sql = "select foeid from entidade.formacaoentidade where entid = $entid ";
				$foeid = $db->pegaUm($sql);
				$obFormacaoEntidade = new FormacaoEntidade($foeid);
				$obFormacaoEntidade->entid = $entid;
				$obFormacaoEntidade->tfoid = $_POST["comboFormacao_$count"];
				$obFormacaoEntidade->tfodscoutros = substr($_POST["dsFormacao_$count"],0,200); 
				/* ($_POST["dsFormacao_$count"] == 'undefined' ) ? "" : $_POST["dsFormacao_$count"]; */
				$obFormacaoEntidade->salvar();
				
				$sql = "select vpeid from entidade.vinculoprofissionalentidade where entid = $entid ";
				$vpeid = $db->pegaUm($sql);
				$obVinculoProfissionalEntidade = new VinculoProfissionalEntidade($vpeid);
				$obVinculoProfissionalEntidade->entid = $entid;
				$obVinculoProfissionalEntidade->tvpid = $_POST["comboVinculoProfissional_$count"];
				$obVinculoProfissionalEntidade->tvpdscoutros = ($_POST["dsVinculoProfissional_$count"] == 'undefined' ) ? "" : $_POST["dsVinculoProfissional_$count"];
				$obVinculoProfissionalEntidade->salvar();
				
				$db->commit();
				unset($obEntidade);
				unset($obComposicaoPessoa);
				unset($obFormacaoEntidade);
				unset($obVinculoProfissionalEntidade);
				
			} else {
				$obComposicaoSubacao = new ComposicaoSubacao();
				
				$eescpf = "coscpf_$count";
				$polid = "polid_$count";
				$eesentescola = "entid_$count";
				
				if( $_REQUEST["coscpf_$count"] ){
					$obComposicaoSubacao->cosdsc = "";
					$obComposicaoSubacao->sbaid = $_REQUEST['sbaid'];
					$obComposicaoSubacao->cosano = $_REQUEST['cosano'];
					//$obComposicaoSubacao->undid = $_REQUEST['undid'];
					$obComposicaoSubacao->cosid = $_REQUEST['cosid'][$count];
					
					$obComposicaoSubacao->salvar();
					$obComposicaoSubacao->commit();
	
					unset( $obComposicaoSubacao );
				}
			}
		}		
		$dadosSalvos = true;
		die();
	}
	else{
		$_REQUEST['cosid'] = ($_REQUEST['cosid']) ? $_REQUEST['cosid'] : array(); 
	    if ($_REQUEST['cosqtd'] && $_REQUEST['cosvlruni']) {
	        $ins = "INSERT INTO cte.composicaosubacao (
	                    sbaid,
	                    cosdsc,
	                    unddid,
	                    cosano,
	                    cosqtd,
	                    cosvlruni
	                ) VALUES (%d, '%s', %d, %d, %f, %f)";
	    
	        $upd = "UPDATE cte.composicaosubacao SET
	                    cosdsc    = '%s',
	                    unddid    = %d,
	                    cosano    = %d,
	                    cosqtd    = %f,
	                    cosvlruni = %f
	                WHERE
	                    cosid     = %d";
	
	        foreach ($_REQUEST['cosid'] as $k => $cosid) {
	            if (trim($_REQUEST['cosdsc'][$k]) == '')
	                continue;
	
	            if ($cosid == '') {
	                $db->executar(sprintf($ins, (integer) $_REQUEST['sbaid'],
	                                            (string)  $_REQUEST['cosdsc'][$k],
	                                            (integer) $_REQUEST['unddid'][$k],
	                                            (integer) $_REQUEST['cosano'],
	                                            (integer) $_REQUEST['cosqtd'][$k],
	                                            (integer) $_REQUEST['cosvlruni'][$k]));
	            } else {
	                $db->executar(sprintf($upd, (string)  $_REQUEST['cosdsc'][$k],
	                                            (integer) $_REQUEST['unddid'][$k],
	                                            (integer) $_REQUEST['cosano'],
	                                            (integer) $cosid,
	                                            (integer) $_REQUEST['cosqtd'][$k],
	                                            (integer) $_REQUEST['cosvlruni'][$k]));
	            }
	        }
	    } else {
	        $ins = "INSERT INTO cte.composicaosubacao (
	                    sbaid,
	                    cosdsc,
	                    unddid,
	                    cosano
	                ) VALUES (%d, '%s', %d, %d)";
	    
	        $upd = "UPDATE cte.composicaosubacao SET
	                    cosdsc = '%s',
	                    unddid = %d,
	                    cosano = %d
	                WHERE
	                    cosid  = %d";
	
	        foreach ($_REQUEST['cosid'] as $k => $cosid) {
	            if (trim($_REQUEST['cosdsc'][$k]) == '')
	                continue;
	
	            if ($cosid == '') {
	                $db->executar(sprintf($ins, (integer) $_REQUEST['sbaid'],
	                                            (string)  $_REQUEST['cosdsc'][$k],
	                                            (integer) $_REQUEST['unddid'][$k],
	                                            (integer) $_REQUEST['cosano']));
	            } else {
	                $db->executar(sprintf($upd, (string)  $_REQUEST['cosdsc'][$k],
	                                            (integer) $_REQUEST['unddid'][$k],
	                                            (integer) $_REQUEST['cosano'],
	                                            (integer) $cosid));
	            }
	        }
	    }
	    $db->commit();
	    $dadosSalvos = true;
    }
}


$sql = '
SELECT
    il.labid
FROM
    cte.laboratorio l
INNER JOIN
    cte.itenslaboratorio il ON l.labid = il.labid
WHERE
    l.undid = (SELECT
                   undid
               FROM
                   cte.subacaoindicador
               WHERE
                   sbaid = ' . (integer) $_REQUEST['sbaid'] . ')';;

$labid = $db->pegaUm($sql);


?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/prototype.js"></script>

    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>

    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
        
		.comboEstilo{
			border-right: #888888 1px solid;
		    border-top: #888888 1px solid;
		    font-size: 10px;
		    vertical-align: middle;
		    border-left: #000000 3px solid;
		    border-bottom: #888888 1px solid;
		    font-family: verdana;
		    background-color: #ffffff;
		    width:130px;
		}
        
    </style>
  </head>
  <body>
    <div id="loader-container">
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>
    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadItensComposicao" onSubmit="return validar(); window.close();"> 
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
        <?php 
        $arValidaCPF = in_array( $undid, $arUnidades );
        $ValidaCPF = $arValidaCPF ? "dados dos cursistas" : "itens de composição"; 
        ?>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center"><?=ucwords($ValidaCPF);?></div>
          <div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
            <table id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
			  <?php
			  if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica ){
			  //if( true ){ ?>
	              <colgroup>
	                <col width="2%" />
	                <col width="10%" />
	                <col width="10%" />
	                <col width="15%" />
	                <col width="15%" />
	              </colgroup>
	              <tr style="text-align: center">
	              	<td style="padding: 2px; font-weight: bold;">&nbsp;</td>
	                <td style="padding: 2px; font-weight: bold;">CPF<input type="hidden" name="boCPF" value="1" /></td>
	                <td style="padding: 2px; font-weight: bold;">Nome</td>
	                <td style="padding: 2px; font-weight: bold;">Vínculo</td>
	                <td style="padding: 2px; font-weight: bold;">Formação</td>
	              </tr>
			  <?php }
			  else{ ?>
	              <colgroup>
	                <col width="10%" />
	                <col width="50%" />
	                <col width="40%" />
	              </colgroup>
	              <tr style="text-align: center">
	                <td style="padding: 2px; font-weight: bold;">&nbsp;</td>
	                <td style="padding: 2px; font-weight: bold;">Identificação do Item</td>
	                <td style="padding: 2px; font-weight: bold;">Unidade de Medida</td>
	              </tr>
			  	
			  <?php }?>
            </table>
          </div>
          <div style="padding: 5px 0px 0px 5px">
<?php
if ($labid)
    echo '            <span style="cursor:pointer" id="linkInserirItem" onclick="return inserirLaboratorio();"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir '.$ValidaCPF.'</span>';
else{
	// Condição não definida
//	if( true ){
	if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica ){
	    echo '            <span style="cursor:pointer" id="linkInserirItem" onclick="return carregarItensCPF(\'\', \'\',\'\',\'\',\'\',\'\',\'\',\'\' );"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir '.$ValidaCPF.'</span>';
	}
	else{
	    echo '            <span style="cursor:pointer" id="linkInserirItem" onclick="return adicionarItemComposicao(\'\',\'\',\'\',',$_REQUEST['cosano'],',true);"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir '.$ValidaCPF.'</span>';
	}
}

echo "\n";
?>
          </div><br />
        </td>
      </tr>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <span style="display: block;font-weight:bold;font-size:110%;margin-bottom:5px;color:red">Atenção! Itens sem identifição serão descartados</span>
          <input type="submit" name="btnGravar" value="Gravar" />
         <!-- <input type="button" name="btnFechar" value="Fechar" onclick="fecharJanela();" />  -->
        </td>
      </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript"><!--
  
    this._closeWindows = false;
    var _modificado    = false;

    /**
     * 
     */
    function removerItemComposicao(linha, cosid)
    {
        var linha = $(linha);
        linha.style.backgroundColor = 'rgb(255,255,210)';
        linha.style.backgroundColor = '#DAE0D2';

        if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
            linha.style.backgroundColor = '#f0f0f0';
        } else {
            $('loader-container').show();
            try {
                if (cosid != '') {
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: 'excluir=' + cosid,
                                                   onComplete: function(r)
                                                   {
                                                       //if (r.responseText == 'null')
                                                           linha.parentNode.removeChild(linha);
                                                       //else
                                                       //    alert('Não foi possível excluir o item.');
                                                           linha.style.backgroundColor = '#f0f0f0';
                                                   }
                                               });
                } else {
                    linha.parentNode.removeChild(linha);
                }
            } catch (e) {
            }

            $('loader-container').hide();
        }

        return false;
    }

    var labInserido = false;
    function inserirLaboratorio()
    {
        if (labInserido)
            return false;

        new Ajax.Request(window.location.href, {
                         method: 'post',
                         parameters: 'inserirItensLaboratorio=1',
                         onComplete: function(res)
                         {
                             eval(res.responseText);

                             for (var i = 0; i < itensLaboratorio.length; i++) {
                                 adicionarItemComposicao(itensLaboratorio[i].cosid,
                                                         itensLaboratorio[i].cosdsc,
                                                         itensLaboratorio[i].unddid,
                                                         itensLaboratorio[i].cosano,
                                                         itensLaboratorio[i].modificado,
                                                         itensLaboratorio[i].cosqtd,
                                                         itensLaboratorio[i].cosvlruni);
                             }
                         }
        });

        labInserido = true;
    }

    /**
     * 
     */
    function adicionarItemComposicao(cosid, cosdsc, unddid, cosano, modificado, cosqtd, cosvlruni)
    {
        var input_cosdsc;
        var input_unddid;

        var btnExcluirItemComposicao     = document.createElement('img');
        btnExcluirItemComposicao.src     = '/imagens/excluir.gif';
        btnExcluirItemComposicao.onclick = function()
        {
            if (labInserido)
                alert('Não é possível excluir itens de laboratórios.');
            else
                removerItemComposicao(this.parentNode.parentNode.getAttribute("id"), cosid);
        }

        input_cosdsc    = document.createElement('input');
        input_cosdsc.setAttribute('type', 'text');
        input_cosdsc.setAttribute('class', 'normal');
        input_cosdsc.setAttribute('name', 'cosdsc[]');
        input_cosdsc.setAttribute('maxlength', '100');
        input_cosdsc.setAttribute('size', '30');
        input_cosdsc.setAttribute('value', cosdsc);

        input_cosid     = document.createElement('input');
        input_cosid.setAttribute('type', 'hidden');
        input_cosid.setAttribute('name', 'cosid[]');
        input_cosid.setAttribute('value', cosid);

        input_unddid   = document.createElement('select');
        input_unddid.setAttribute('class', 'CampoEstilo');
        input_unddid.setAttribute('name', 'unddid[]');
        <?php
    $res = (array) $db->carregar('select unddid, undddsc from cte.unidademedidadetalhamento order by undddsc');

    while (list(, $item) = each($res)) {
        $item = array_map('addslashes', $item);
        echo 'selected=unddid==' , $item["unddid"]
            , ';input_unddid.options[input_unddid.options.length]=new Option(\'' , $item["undddsc"] , '\',\'' , $item["unddid"] , '\',selected,selected);';
    }

?>

        input_cosdsc.setAttribute('value', cosdsc);

        var tabelaItensComposicaoSubAcao = $('itensComposicaoSubAcao');
        var novaLinha                    = tabelaItensComposicaoSubAcao.insertRow(tabelaItensComposicaoSubAcao.rows.length);
        novaLinha.id                     = 'linhaItensComposicaoSubAcao' + tabelaItensComposicaoSubAcao.rows.length + 1;
        novaLinha.style.textAlign        = 'center'
        novaLinha.style.backgroundColor  = '#f0f0f0';

        /**
         * Botao excluir
         */
        var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula0.appendChild(btnExcluirItemComposicao);

        /**
         * Descricao
         */
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        input_cosdsc.setAttribute('id', novaLinha.cells.length);
        novaCelula2.appendChild(input_cosdsc);
        novaCelula2.appendChild(input_cosid);

        /**
         * Unidade de medida
         */
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula3.appendChild(input_unddid);

        if (cosqtd) {
            var input_cosqtd = document.createElement('input');
            input_cosqtd.setAttribute('type', 'hidden');
            input_cosqtd.setAttribute('name', 'cosqtd[]');
            input_cosqtd.setAttribute('value', cosqtd);

            novaCelula3.appendChild(input_cosqtd);
        }

        if (cosvlruni) {
            var input_cosvlruni = document.createElement('input');
            input_cosvlruni.setAttribute('type', 'hidden');
            input_cosvlruni.setAttribute('name', 'cosvlruni[]');
            input_cosvlruni.setAttribute('value', cosvlruni);

            novaCelula3.appendChild(input_cosvlruni);
        }


        if (cosdsc == null)
            input_cosdsc.select();

        _modificado = modificado;

        labInserido = true;
        return false;
    }


    function carregarItensComposicao(){<?php
if ($_REQUEST['sbaid'] && $_REQUEST['cosano']) {
	
	if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica ){
		$sql = 'SELECT 
				    cp.cspid, 
			        e.entid, 
			        e.entnumcpfcnpj, 
			        e.entnome,
			        tvp.tvpid,
			        CASE 
			            WHEN tvp.tvpid = 4
			        THEN vpe.tvpdscoutros
			            ELSE tvp.tvpdsc
			        END AS vinculo,
			        tf.tfoid,
			        CASE 
			            WHEN tf.tfoid = 7 
			        THEN fe.tfodscoutros
			            ELSE tf.tfodsc
			        END AS formacao
			    FROM cte.composicaopessoa cp
			        inner join entidade.entidade e on cp.entid = e.entid
			        left join entidade.formacaoentidade fe on e.entid = fe.entid
			        inner join public.tipoformacao tf on fe.tfoid =  tf.tfoid
			        left join entidade.vinculoprofissionalentidade vpe on e.entid = vpe.entid
			        inner join public.tipovinculoprofissional tvp on vpe.tvpid =  tvp.tvpid
                            WHERE cp.sbaid  = ' . $_REQUEST['sbaid']   . '
                            AND cp.cspano   = ' . $_REQUEST['cosano'];
	} else {
		$sql = 'SELECT cs.cosid, cs.cosdsc, um.unddid
                            FROM cte.composicaosubacao cs
                            	left JOIN cte.unidademedidadetalhamento um ON cs.unddid = um.unddid
                            WHERE cs.sbaid  = ' . $_REQUEST['sbaid']   . '
                            AND cs.cosano   = ' . $_REQUEST['cosano']  . '
                            ORDER BY cs.cosord ASC, cs.cosdsc ASC';
	}
	$itens = $db->carregar($sql);
	
    if (!is_array($itens)) {
        echo 'return false;';
    } else {
        while (list(, $item) = each($itens)) {
            $item = array_map('addslashes', $item);
            // Condição não definida
            if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica ){
	            echo "carregarItensCPF( " , $item['cspid'] , "," , $_REQUEST['sbaid'] , ",'" , $item['entnumcpfcnpj'] , "','" , $item['entnome'] , "','" , $item['tvpid'] ,"','" , $item['vinculo'] , "','" , $item['tfoid'] ,"','" , $item['formacao'] , "');";
            }
            else{
	            echo "adicionarItemComposicao(" , $item['cosid'] , ",'" , $item['cosdsc'] , "','" , $item['unddid'] , "'," , $_REQUEST['cosano'] , ",false);";
            }	
        }
    }
} else {
    echo 'return false;';
}



?>}/*!@ !function carregarItensComposicao() */

    carregarItensComposicao();
    $('loader-container').hide();

    function fecharJanela()
    {
        if (_modificado) {
            if (!confirm('Existem dados que não foram salvos.\nDeseja fechar a janela sem salvá-los?'))
                return false;
        }

        window.close();
    }

    window.onunload = function()
    {
    	var sbaid 			 = <?php echo $_REQUEST["sbaid"]; ?>;
    	var parsubacaoanocrt = <?php echo $_REQUEST["cosano"]; ?>;
    	var sbaidpai  =  <?php echo $sbaidPai;  ?>;
    	var anoconvenio = <?php echo $anoAditivo;  ?>;
    	var aditivo		= <?php echo $aditivo; ?>
        //window.opener.carregarItensComposicao();
        window.opener.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&parsubacaoanocrt='+ parsubacaoanocrt +'&sbaid='+ sbaid + '&sbaidpai='+ sbaidpai + '&anoconvenio='+ anoconvenio + '&ad='+aditivo;
    }
    
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

function removerLinha( idLinha, cosid, cspid ){
	
	if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
		return false;
	}

	new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&reqt=<?php echo $reqt; ?>&req=removerItemComposicao&cosid=' + cosid+'&cspid='+cspid
                                   });
                               	
	var tabela 	= document.getElementById( "itensComposicaoSubAcao" );
	var linha 	= document.getElementById( idLinha );

	tabela.deleteRow( linha.rowIndex );

}

function procurarNome( cpf, qtd ){
	var data = '';
	var comp = new dCPF();
	var arNome = document.getElementsByName( 'cosnome_' + qtd );
	
	comp.buscarDados( cpf );
	if (comp.dados.no_pessoa_rf != ''){
		arNome[0].value = comp.dados.no_pessoa_rf;
		arNome[0].readOnly = true;
	}
}

function eventosIE( obj, tipo ){
	
	switch( tipo ){
		case('1'): MouseBlur(obj); validarcoscpf( obj ); break;
		case('2'): MouseOut(obj); break;
		case('3'): MouseClick(obj); obj.select(); break;
		case('4'): MouseOver(obj); break;
		case('5'): obj.value=mascaraglobal('###.###.###-##',obj.value); break;
	}
	
}

function eventosIECPF( obj, qtd ){
	procurarNome( obj.value, qtd );
}

function validarcoscpf( obj ){
	
	if( obj.value ){
		if( !validar_cpf( obj.value  ) ){
			alert( "CPF inválido!\nFavor informar um cpf válido!" );
			obj.value = "";	
		}
	}
}

function carregarItensCPF( cspid, sbaid, coscpf, cosnome, tvpid , vinculo, tfoid, formacao ){

	var tabela 	= document.getElementById( "itensComposicaoSubAcao" );
	var qtdLinha = tabela.rows.length;
	
	var anoRef = <?=$_REQUEST['cosano'];?>;	
	var qtdPrevista = window.opener.document.getElementById('sptunt_'+anoRef).value;
	
	var qtd = qtdLinha - 1;
	
	//alert(qtdPrevista+" - "+qtdLinha+" - "+anoRef);
	if(qtdLinha > qtdPrevista){
		if(!confirm("A quantidade de itens é maior que a prevista!\nDeseja continuar?")){
			return false;
		}
	}
											
	linha = tabela.insertRow( tabela.rows.length );
	
	linha.setAttribute( "id", "linha_" + qtd );
	linha.setAttribute( "style", "background: #f5f5f5;" );
	linha.setAttribute( "align", "center" );
											
	var img = document.createElement( "img" );
	img.setAttribute( "src", "/imagens/excluir.gif" );
	img.setAttribute( "title", "Excluir" );
	img.setAttribute( "alt", "Excluir" );

	// OnBlur
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		img.setAttribute( "onclick", "removerLinha( 'linha_" + qtd + "', '', '"+ cspid +"' )" );
	}
	else{ // IE
		img.attachEvent( "onclick", function() { removerLinha( "linha_" + qtd , '', cspid ) } );
	}
				
	/*var input_cosid = document.createElement('input');
	input_cosid.setAttribute('type', 'hidden');
	input_cosid.setAttribute('name', 'cosid[]');
	input_cosid.setAttribute('value', cosid);*/
        									
	/*var input_sbaid     = document.createElement('input');
	input_sbaid.setAttribute('type', 'hidden');
	input_sbaid.setAttribute('name', 'sbaid[]');
	input_sbaid.setAttribute('value', sbaid);*/									
						
	var cpf = document.createElement( "input" );
	cpf.setAttribute( "type", "text" );
	cpf.setAttribute( "name", "coscpf_" + qtd );
	cpf.setAttribute( "id", "coscpf_" + qtd );
	cpf.setAttribute( "style", "width: 21ex;" );
	cpf.setAttribute( "size", "19" );
	coscpfValorFormatado = "";
	if(coscpf){
		coscpfValorFormatado = mascaraglobal('###.###.###-##',coscpf)
	}
	cpf.setAttribute( "value", coscpfValorFormatado );
	cpf.setAttribute('onkeyup', "this.value=mascaraglobal('###.###.###-##',this.value);");
	cpf.className = "normal";

	// OnBlur
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		cpf.setAttribute( "onblur", "MouseBlur(this); validarcoscpf( this );" );
	}
	else{ // IE
		cpf.attachEvent( "onblur", function() { eventosIE( document.getElementById( 'coscpf_' + qtd ), "1" ); } );
	}
	
	// OnMouseOut
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		cpf.setAttribute( "onmouseout", "MouseOut(this);" );
	}
	else{ // IE
		cpf.attachEvent( "onmouseout", function() { eventosIE( document.getElementById( 'coscpf_' + qtd ), "2" ); } );
	}
	
	// OnFocus
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		cpf.setAttribute( "onfocus", "MouseClick(this); this.select();" );
	}
	else{ // IE

		cpf.attachEvent( "onfocus", function() { eventosIE( document.getElementById( 'coscpf_' + qtd ), "3" ); } );
	}
	
	// OnMouseOver
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		cpf.setAttribute( "onmouseover", "MouseOver(this);" );
	}
	else{ // IE
		cpf.attachEvent( "onmouseover", function() { eventosIE( document.getElementById( 'coscpf_' + qtd ), "4" ); } );
	}
	
	// OnKeyUp
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		cpf.setAttribute( "onkeyup", "this.value=mascaraglobal('###.###.###-##',this.value);" );
	}
	else{ // IE
		cpf.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'coscpf_' + qtd ), "5" ); } );
	}
	
	// OnChange
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		cpf.setAttribute( "onchange", "procurarNome( this.value, "+ qtd +" )" );
		cpf.setAttribute( "onblur", "procurarNome( this.value, "+ qtd +" )" );
	}
	else{ // IE
		cpf.attachEvent( "onkeyup", function() { eventosIECPF( document.getElementById( 'coscpf_' + qtd ), qtd ); } );
	}
	
	// OnKeyUp
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		cpf.setAttribute( "onkeyup", "this.value=mascaraglobal('###.###.###-##',this.value);" );
	}
	else{ // IE
		cpf.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'coscpf_' + qtd ), "5" ); } );
	}

	var nome = document.createElement( "input" );
	nome.setAttribute( "type", "text" );
	//nome.setAttribute( "disabled", "disabled" );
	nome.setAttribute( "name", "cosnome_" + qtd );
	nome.setAttribute( "id", "cosnome_" + qtd );
	nome.setAttribute( "style", "width: 33ex;" );
	nome.setAttribute( "size", "30" );
	nome.setAttribute( "value", cosnome );
	nome.className = "disabled";

	/**
	* Tipo Vinculo Profissional
	**/		
	var comboVinculoProfissional = document.createElement( 'select' );
	comboVinculoProfissional.setAttribute( "name", "comboVinculoProfissional_" + qtd );
	comboVinculoProfissional.setAttribute( "id", "comboVinculoProfissional_" + qtd );
	comboVinculoProfissional.className = 'comboEstilo';
	// OnKeyUp
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		comboVinculoProfissional.setAttribute( "onchange", "alternarExibicaoVinculo( " + qtd + ", 1 );" );
	}
	else{ // IE
		comboVinculoProfissional.attachEvent( "onchange", function() { alternarExibicaoVinculo( qtd, 1 ); } );
	}
	
	var option = document.createElement( 'option' );
	option.setAttribute( 'value', '' );
	option.innerHTML = 'Selecione';
	comboVinculoProfissional.appendChild( option );
	
	<?php foreach( $coTipoVinculoProfissional as $arTipoVinculoProfissional ){ ?>
	var option = document.createElement( 'option' );
	option.setAttribute( 'value', '<?php echo $arTipoVinculoProfissional["tvpid"]; ?>' );
	if( '<?php echo $arTipoVinculoProfissional["tvpid"]; ?>' == tvpid ){
		option.setAttribute( 'selected', 'selected' );
	}
	option.innerHTML = '<?php echo $arTipoVinculoProfissional["tvpdsc"]; ?>';
	comboVinculoProfissional.appendChild( option );
	<?php } ?>

	campoVinculoProfissional = document.createElement( "input" );
	campoVinculoProfissional.setAttribute( "type", "text" );
	campoVinculoProfissional.setAttribute( "name", "dsVinculoProfissional_" + qtd );
	campoVinculoProfissional.setAttribute( "id", "dsVinculoProfissional_" + qtd );
	campoVinculoProfissional.setAttribute( "value", vinculo );
	campoVinculoProfissional.setAttribute( "style", "width: 30ex; display: none" );
	campoVinculoProfissional.className = "objetoOculto";
	//campoVinculoProfissional.setAttribute( "size", "20" );
	
	link = document.createElement( "a" );
	link.setAttribute( "href", "javascript: alternarExibicaoVinculo( " + qtd + ", 2 )" );
	link.setAttribute( "id", "linkVoltar_" + qtd );
	link.setAttribute( "style", "margin-left: 20px; width: 50ex; display: none" );
	link.className = "objetoOculto";
	link.innerHTML = "Exibir Opções";
	

	/**
	* Tipo Formação
	**/
	var comboFormacao = document.createElement( 'select' );
	comboFormacao.setAttribute( "name", "comboFormacao_" + qtd );
	comboFormacao.setAttribute( "id", "comboFormacao_" + qtd );
	comboFormacao.className = 'comboEstilo';
	// OnKeyUp
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		comboFormacao.setAttribute( "onchange", "alternarExibicaoFormacao( " + qtd + ", 1 );" );
	}
	else{ // IE
		comboFormacao.attachEvent( "onchange", function() { alternarExibicaoFormacao( qtd, 1 ); } );
	}		
	
	var option = document.createElement( 'option' );
	option.setAttribute( 'value', '' );
	option.innerHTML = 'Selecione';
	comboFormacao.appendChild( option );
	
	
	<?php foreach( $coTipoFormacao as $arTipoFormacao ){ ?>
		var option = document.createElement( 'option' );
		option.setAttribute( 'value', '<?php echo $arTipoFormacao["tfoid"]; ?>' );
		if( '<?php echo $arTipoFormacao["tfoid"]; ?>' == tfoid ){
			option.setAttribute( 'selected', 'selected' );
		}
		option.innerHTML = '<?php echo $arTipoFormacao["tfodsc"]; ?>';
		comboFormacao.appendChild( option );
	<?php } ?>

	campoFormacao = document.createElement( "input" );
	campoFormacao.setAttribute( "type", "text" );
	campoFormacao.setAttribute( "name", "dsFormacao_" + qtd );
	campoFormacao.setAttribute( "id", "dsFormacao_" + qtd );
	campoFormacao.setAttribute( "value", formacao );
	campoFormacao.setAttribute( "maxlength", "200" );
	campoFormacao.setAttribute( "style", "width: 30ex; display: none" );
	campoFormacao.className = "objetoOculto";
	//campoFormacao.setAttribute( "size", "20" );
	
	link2 = document.createElement( "a" );
	link2.setAttribute( "href", "javascript: alternarExibicaoFormacao( " + qtd + ", 2 )" );
	link2.setAttribute( "id", "linkVoltar2_" + qtd );
	link2.setAttribute( "style", "margin-left: 20px; width: 30ex; display: none" );
	link2.className = "objetoOculto";
	link2.innerHTML = "Exibir Opções";	
	
	var entid = document.createElement( "input" );
	entid.setAttribute( "type", "hidden" );
	entid.setAttribute( "name", "entid_" + qtd );
	entid.setAttribute( "value", qtd );
	
	var arcos = document.createElement( "input" );
	arcos.setAttribute( "type", "hidden" );
	arcos.setAttribute( "name", "arcos[]" );
	arcos.setAttribute( "value", qtd );
                                       
	celula = linha.insertCell( 0 );
	celula.appendChild( img );
	celula.appendChild( arcos );
	
	var imgObrigatorio = document.createElement( "img" );
	imgObrigatorio.setAttribute( "src", "../imagens/obrig.gif" );
	imgObrigatorio.setAttribute( "title", "Indica campo obrigatório." );

	var imgObrigatorio2 = document.createElement( "img" );
	imgObrigatorio2.setAttribute( "src", "../imagens/obrig.gif" );
	imgObrigatorio2.setAttribute( "title", "Indica campo obrigatório." );

	var imgObrigatorio3 = document.createElement( "img" );
	imgObrigatorio3.setAttribute( "src", "../imagens/obrig.gif" );
	imgObrigatorio3.setAttribute( "title", "Indica campo obrigatório." );												
	
	celula = linha.insertCell( 1 );
	//celula.appendChild( input_cosid );
	//celula.appendChild( input_sbaid );
	celula.appendChild( cpf );
	celula.appendChild( imgObrigatorio );
	
	celula = linha.insertCell( 2 );
	celula.appendChild( nome );
	
	celula = linha.insertCell( 3 );
	celula.appendChild( comboVinculoProfissional );
	celula.appendChild( campoVinculoProfissional );
	celula.appendChild( imgObrigatorio2 );
	celula.appendChild( link );
	
	celula = linha.insertCell( 4 );
	celula.appendChild( comboFormacao );
	celula.appendChild( campoFormacao );
	celula.appendChild( imgObrigatorio3 );
	celula.appendChild( link2 );

	if(tvpid == 4 ) { 
		alternarExibicaoVinculo( qtd, 1 ); 
	} else {
		alternarExibicaoVinculo( qtd, 0 );
	}

	if(tfoid == 7 ) { 
		alternarExibicaoFormacao( qtd, 1 ); 
	} else {
		alternarExibicaoFormacao( qtd, 0 );
	}
											
}


function alternarExibicaoVinculo( qtd, tipo ){
	var comboVinculoProfissional = document.getElementById( 'comboVinculoProfissional_'+qtd );
	var dsVinculoProfissional = document.getElementById( 'dsVinculoProfissional_'+qtd );
	var link = document.getElementById( 'linkVoltar_'+qtd );
	
	if( tipo == 1 ){
		if( comboVinculoProfissional.value == 4 ){
			dsVinculoProfissional.style.display = "";
			dsVinculoProfissional.className = "";
			if(dsVinculoProfissional.value == 'undefined'){
				dsVinculoProfissional.value = "";
			}
			link.style.display = "";
			comboVinculoProfissional.style.display = "none";
			link.className = "";
		}
	}
	else{
		dsVinculoProfissional.style.display = "none";
		link.style.display = "none";
		link.className = "objetoOculto";
		if(tipo != 0){
			comboVinculoProfissional.style.display = "";
			comboVinculoProfissional.value = "";
			dsVinculoProfissional.value = "";
		}
	}
}

function alternarExibicaoFormacao( qtd, tipo ){
	var comboFormacao = document.getElementById( 'comboFormacao_'+qtd );
	var dsFormacao = document.getElementById( 'dsFormacao_'+qtd );
	var link = document.getElementById( 'linkVoltar2_'+qtd );
	
	if( tipo == 1 ){
		if( comboFormacao.value == 7 ){
			dsFormacao.style.display = "";
			dsFormacao.className = "";
			if(dsFormacao.value == 'undefined'){
				dsFormacao.value = "";
			}
			link.style.display = "";
			comboFormacao.style.display = "none";
			link.className = "";
		}
	}	
	else{
		dsFormacao.style.display = "none";
		link.style.display = "none";
		link.className = "objetoOculto";
		if(tipo != 0){
			comboFormacao.style.display = "";
			comboFormacao.value = "";
			dsFormacao.value = "";
		}
	}
}

function validar(){
	var arcos = document.getElementsByName( 'arcos[]' );
	var boFormacao = false;
	var boVinculo = false;
	var boCpf = false;
	var boNome = false;
	
	for( i=0; i < arcos.length; i++ ){
		var comboFormacao = document.getElementById( 'comboFormacao_'+i );
		var comboVinculoProfissional = document.getElementById( 'comboVinculoProfissional_'+i );
		var coscpf = document.getElementById( 'coscpf_'+i );
		var cosnome = document.getElementById( 'cosnome_'+i );
		
		if(comboFormacao.value == ""){
			boFormacao = true;
		}
		if(comboVinculoProfissional.value == ""){
			boVinculo = true;
		}
		if(coscpf.value == ""){
			boCpf = true;
		}
		if(cosnome.value == ""){
			boNome = true;
		}
		
		
    }

    if(boFormacao || boVinculo || boCpf || boNome){
        if(boCpf){
        	alert('O campo CPF é obrigatório.');
        }
        if(boNome){
        	alert('O campo Nome é obrigatório.');
        }
        if(boVinculo){
        	alert('O campo Vínculo é obrigatório.');
        }
        if(boFormacao){
        	alert('O campo Formação é obrigatório.');
        }
		return false;
    }
	return true;
}
  
  --></script>
</html>