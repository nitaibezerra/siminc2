<?php
require_once '../../global/config.inc';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']){
	$_SESSION['usucpf'] = '';
	$_SESSION['usucpforigem'] = '';
	$auxusucpf = '';
	$auxusucpforigem = '';
}else{
	$auxusucpf = $_SESSION['usucpf'];
	$auxusucpforigem = $_SESSION['usucpforigem'];
}

$_SESSION["sisid"] = 183;

include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$db = new cls_banco();

$lista = array();

// Classes
include_once APPRAIZ . "includes/PdfParser/Parser.php";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'sase/classes/Assessoramento.class.inc';
include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';

function utf8_fix($string) {
	$text = htmlentities( $string, ENT_COMPAT, 'UTF-8' );
	return html_entity_decode($text, ENT_COMPAT, 'ISO-8859-1');
}

function str_contains($haystack, $needle, $ignoreCase = false) {
	if ($ignoreCase) {
		$haystack = strtolower($haystack);
		$needle   = strtolower($needle);
	}
	$needlePos = strpos($haystack, $needle);
	return ($needlePos === false ? false : ($needlePos+1));
}

if ($_REQUEST['acao']) {
    switch($_REQUEST['acao']) {
        case 'download':
    		if ($_GET['abrangencia'] == 'M') {
				$sql = "select assleipne as lei from sase.assessoramento where assleipne = '{$_GET['lei']}'";
				$arqid = $db->pegaUm($sql);
				$file = new FilesSimec('assessoramento', array(), 'sase');
			} else {
				$sql = "select aseleipne as lei from sase.assessoramentoestado where aseleipne = '{$_GET['lei']}'";
				$arqid = $db->pegaUm($sql);
				$file = new FilesSimec('assessoramentoestado', array(), 'sase');
			}

            if ($arqid) {
                ob_clean();
                $arquivo = $file->getDownloadArquivo($arqid);
                echo $arquivo;
            }
			exit();
		break;
		case 'pesquisarLei':
			if (isset($_POST['texto']) && !empty($_POST['texto'])) {
				if ($_POST['abragencia'] == 'M') {
					$municipios = isset($_POST['municipio']) ? implode(',', str_replace('\\', '', $_POST['municipio'])) : null;
					$sql = "select m.mundescricao, 'Municipal' as regiao, a.estuf, a.assleipne as lei 
							from sase.assessoramento a
							inner join territorios.municipio m on m.muncod = a.muncod ";
					
					if ($municipios) {
						$sql.= " where CAST(coalesce(a.muncod, '0') AS integer) in ({$municipios}) ";
					}
					$resultado = $db->carregar($sql);
				} else if ($_POST['abragencia'] == 'E') {
                    $estados = isset($_POST['uf']) ? implode(',', str_replace('\\', '', $_POST['uf'])) : null;
					$sql = "select '' as mundescricao, 'Estadual' as regiao, estuf, aseleipne as lei 
							from sase.assessoramentoestado";
                    if($estados){
                        $sql .= " where CAST(coalesce(estuf, '0') as integer) in ({$estados})";
                    }
					$resultado = $db->carregar($sql);
				} else {
					$municipios = isset($_POST['municipio']) ? implode(',', str_replace('\\', '', $_POST['municipio'])) : null;
                    $estados = isset($_POST['uf']) ? implode(',', str_replace('\\', '', $_POST['uf'])) : null;
					$sql = "select m.mundescricao, 'Municipal' as regiao, a.estuf, a.assleipne as lei 
							from sase.assessoramento a
							inner join territorios.municipio m on m.muncod = a.muncod ";
					
					if ($municipios) {
						$sql.= " where CAST(coalesce(a.muncod, '0') AS integer) in ({$municipios}) ";
					}

					$sql.= "union "; 
					$sql.= "select '' as mundescricao, 'Estadual' as regiao, estuf, aseleipne as lei from sase.assessoramentoestado";
                    if ($estados){
                        $sql.= " where CAST(coalesce(estuf, '0') AS integer) in ({$estados}) ";
                    }
					$resultado = $db->carregar($sql);
				}
				ver($sql, d);
				$fileMunicipio = new FilesSimec('assessoramento', array(), 'sase');
				$fileEstado = new FilesSimec('assessoramentoestado', array(), 'sase');

				if (count($resultado) > 0) {
					foreach ($resultado as $data) {
						if ($data['lei']) {
							ob_clean();
							if ($data['regiao'] == 'Estadual') {
								$arquivo = $fileEstado->getDadosArquivo($data['lei']);
							} else { 
								$arquivo = $fileMunicipio->getDadosArquivo($data['lei']);
							}
							
							if ($arquivo['caminhofisico']) {
								$object = new PDF2Text();
								$object->setFilename($arquivo['caminhofisico']);
								$object->decodePDF();
								$texto = utf8_fix($object->output());
								
								if (str_contains($_POST['texto'], $texto, true)) {
									$lista[] = $data;
								}
							}
						}
					}
				}
			}
			break;
        case 'carregarMunicipio':
        	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf in ('".implode("','", $_POST['estuf'])."') ORDER BY mundescricao";
        	$resultado = $db->carregar($sql);
        	print simec_json_encode($resultado);
        	exit;
        break;
    }
}
?>

<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css">
<link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
<link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" media="screen">
<link rel="stylesheet" href="../library/chosen-1.0.0/chosen.css" media="screen" >

<style>
	.conteudo-sase{ width:100%; } 
	.chosen-container-single .chosen-single { height: 35px; }
	.chosen-container-multi .chosen-choices { padding: 4px 8px; min-height: 34px; }
	.chosen-container-single .chosen-single div b { background-position: 0px 10px; }
	.document { background: url('document.jpg') no-repeat; height: 164px; padding-top: 15px; }
	.document a { text-decoration: none; color: #000; }
	.region, .law, .description, .municipio, .download {   
		margin: 0px 10px 0px 20px;
    	font-weight: bold;
    	font-size: 12px;
    }
    .municipio { font-size: 9px; }
    .region { font-size: 11px; }
    .description { font-size: 9px; width: 100px; height: 30px; }
    .law { font-size: 10px; text-decoration: underline; }
    .download { font-size: 11px; color: #000; padding-top: 15px; }
    h6 { font-size: 9px; margin-top: 0; }
    #municipio___chosen { width: 100% !important; }
</style>

<script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<script src="/sase/js/functions.js"></script>   

<!-- /dependencias -->
<form id="form-pesquisa" method="post" name="formPesquisarLista" role="form" class="form-horizontal">
    <input type="hidden" name="acao" id="acao" />
	<div class="panel panel-default">
        <div class="panel-heading">
            <h1 class="panel-title">Pesquisa nas Leis do PEE/PME</h1>
            <h6>Esta página permite pesquisar por qualquer assunto nas leis do PEE/PME disponíveis para downlod.</h6>
        </div>
        <div class="panel-body">
        	<form class="form-horizontal" action="sase_filtro.php">
        		<input type="hidden" name="acao" value="pesquisarLei" />
			    <div class="form-group">
			    	<label for="abragencia" class="control-label col-xs-3">Abrangência</label>
			    	<div class="col-xs-9">
						<label class="radio-inline">
							<input type="radio" name="abragencia[]" id="abragencia[1]" class="abragencia" value="E"> Estado
						</label>
						<label class="radio-inline">
							<input type="radio" name="abragencia[]" id="abragencia[2]" class="abragencia" value="M"> Municípios
						</label>
						<label class="radio-inline">
							<input type="radio" name="abragencia[]" id="abragencia[3]" class="abragencia" value="A" checked=""> Ambos
						</label>
					</div>
				</div>
				<div class="form-group">
			        <label for="uf" class="control-label col-xs-3">UF</label>
			        <div class="col-xs-9">
			        	<?php $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf"; ?>
                        <?php $estados = $db->carregar($sql); ?>
                        <select multiple="multiple" data-placeholder="Selecione os municípios" name="uf[]" class="CampoEstilo chosen-select uf" id="uf" style="width: 100%">
                            <option value="">Selecione</option>
                            <?php foreach ($estados as $estado) : ?>
                                <?php $selected = in_array($estado['codigo'], $_REQUEST['uf'] ? $_REQUEST['uf'] : array()) ? 'selected="selected"' : null; ?>
                                <option <?php echo $selected; ?> value="<?php echo $estado['codigo']; ?>"><?php echo $estado['descricao']; ?></option>
                            <?php endforeach; ?>
                        </select>
			      		<?php //$db->monta_combo('uf', $sql, 'S', 'Selecione', '', '', '', '300', 'N', 'uf', '', $_REQUEST['uf'], '', '', 'chosen-select uf'); ?>
			        </div>
			    </div>
    			<div class="form-group" id="divMunicipio">
			        <label for="municipio" class="control-label col-xs-3">Município</label>
			        <div class="col-xs-9">
			        	<?php if ($_REQUEST['uf']) : ?>
                        <?php $estados = isset($_REQUEST['uf']) ? implode("','", $_REQUEST['uf']) : '' ?>
			        	<?php $sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio where estuf in ('{$estados}') ORDER BY mundescricao"; ?>
			        	<?php $municipios = $db->carregar($sql); ?>
			        	<select multiple="multiple" data-placeholder="Selecione os municípios" name="municipio[]" class="CampoEstilo chosen-select" id="municipio" style="width: 100%">
			        		<option value="">Selecione</option>
			        		<?php foreach ($municipios as $municipio) : ?>
			        			<?php $selected = in_array($municipio['codigo'], $_REQUEST['municipio'] ? $_REQUEST['municipio'] : array()) ? 'selected="selected"' : null; ?>
			        			<option <?php echo $selected; ?> value="<?php echo $municipio['codigo']; ?>"><?php echo $municipio['descricao']; ?></option>
			        		<?php endforeach; ?>
						</select>
			      		<?php else: ?>
			        	<select multiple="multiple" data-placeholder="Selecione os municípios" name="municipio[]" class="CampoEstilo chosen-select" id="municipio" style="width: 100%">
			        		<option value="">Selecione</option>
						</select>
						<?php endif; ?>
			        </div>
			    </div>
				<div class="form-group">
			        <label for="texto" class="control-label col-xs-3">Assunto</label>
			        <div class="col-xs-9">
			        	<input type="text" name="texto" value="<?php echo $_REQUEST['texto']?>" placeholder="Assunto para pesquisa" class="form-control">
			        </div>
			    </div>
				<div class="form-group">
					<div class="col-xs-3"></div>
					<div class="col-xs-9">
			            <button type="submit" class="btn btn-primary"><i class="fa fa-search"></i> Pesquisar</button>
                        <button type="button" class="btn btn-primary" onclick="location.reload()">Limpar</button>
			        </div>
				</div>
        	</form>
        </div>
        <div class="panel-body">
        	<?php if (sizeof($lista) > 0) : ?>
	        	<?php foreach ($lista as $lei) : ?>
	        		<div class="col-xs-3 document">
	        			<a target="blank" href="sase_filtro.php?acao=download&abrangencia=<?php echo $lei['mundescricao'] ? 'M' : 'E'; ?>&lei=<?php echo $lei['lei']; ?>">
	        			<p class="law">Lei PEE/PME</p><br/>
	        			<p class="description">
	        				<?php echo $lei['estuf']; ?>
	        				<?php if ($lei['mundescricao']) : ?>
	        					- <?php echo $lei['mundescricao']; ?>
	        				<?php endif; ?>
	        			</p>
	        			<p class="region"><i class="fa fa-map-marker"></i> <?php echo $lei['regiao']; ?></p><br/>
	        			<p class="download"><i class="fa fa-download"></i> Baixar</p>
	        			</a>
	        		</div>
				<?php endforeach; ?>
			<?php else: ?>
				<div class="alert alert-warning">
					Nenhuma lei PEE/PME encontrada para os filtros utilizados. 
				</div>
			<?php endif;?>
        </div>
    </div>
</form>

<script>
	$(document).ready(function() {
		$('.chosen-select').chosen();
		
		$('.uf').change(function(e) {
			e.preventDefault();
	    	var options = $("#municipio");
	    	$.ajax({
				url: 'sase_filtro.php?acao=carregarMunicipio',
				data: {'estuf' : $(this).val()},
				method: 'post',
				success: function (result) {
					options.empty();
					var result = JSON.parse(result);
					$.each(result, function() {
					    options.append(new Option(this.descricao, "'" + this.codigo + "'"));
					});
					options.trigger('chosen:updated');
				}
			});
	    });

        $(".abragencia").click(function(){
            switch ($(this).val()){
                case 'E':
                    $('#divMunicipio').hide();
                    break;
                case 'A':
                case 'M':
                    $('#divMunicipio').show();
                    break;
            }
        });
	})
</script>