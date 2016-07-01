<?php
/**
 *
 */
include "config.inc";
/**
 *
 */

function __autoload($class_name) {
	$arCaminho = array(
						APPRAIZ . "includes/classes/modelo/public/",
						APPRAIZ . "includes/classes/modelo/territorios/",
						APPRAIZ . "includes/classes/modelo/entidade/",
						APPRAIZ . "includes/classes/modelo/obras2/",
						APPRAIZ . "includes/classes/controller/",
						APPRAIZ . "includes/classes/view/",
						APPRAIZ . "includes/classes/html/",
						APPRAIZ . "obras/classe/controller/",
						APPRAIZ . "obras/classe/modelo/"
					  );
					  
	foreach($arCaminho as $caminho){
		$arquivo = $caminho . $class_name . '.class.inc';
		if ( file_exists( $arquivo ) ){
			require_once( $arquivo );
			break;	
		}
	}				  
}

include APPRAIZ . "includes/classes_simec.inc";

include APPRAIZ . "includes/classes/Modelo.class.inc";
include APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";

include APPRAIZ . "par/classes/modelo/PreObra.class.inc";
include APPRAIZ . "includes/classes/Controle.class.inc";
include APPRAIZ . "par/classes/controle/SubacaoControle.class.inc";

include APPRAIZ . "includes/classes/Visao.class.inc";
include APPRAIZ . "includes/classes/modelo/entidade/Endereco.class.inc";

/**
 *
 */
include APPRAIZ . "includes/funcoes.inc";

include APPRAIZ . "www/obras2/_funcoes.php";
include APPRAIZ . "www/obras2/_funcoes_obras_par.php";
include_once APPRAIZ . "www/obras2/_constantes.php";

/**
 *
 * @param type $preid
 * @param type $hash
 * @return int
 */
function validaHash($preid, $hash)
{
    global $db;
    $sql = <<<DML
SELECT case when now()::date <= (hsa.hshdata::date + hst.hstvalidade)::DATE
              then 1
            else 0
       end  AS acessovalido
  FROM obras2.hashacesso hsa
    INNER JOIN obras2.hashacesso_tipo hst USING(hstid)
  WHERE hsa.preid = %d
    AND hsa.hshmd5 = '%s'
DML;
    $sql = sprintf($sql, $preid, $hash);
    $acessovalido = $db->pegaUm($sql);
    if (1 == $acessovalido) {
        return 1;
    }
    return 0;
}

function pegaObraID($preid)
{
    global $db;
    $sql = <<<DML
SELECT pob.obrid
  FROM obras.preobra pob
  WHERE pob.preid = %d
DML;
    $sql = sprintf($sql, $preid);
    return $db->pegaUm($sql);
}

// -----

$db = new cls_banco();

$preid = $_REQUEST['preid'];
$hash = $_REQUEST['hash'];

if (!validaHash($preid, $hash)) {
    echo <<<HTML
<h1 style="text-align:center;color:red">Acesso negado</h1>
HTML;
    exit();
}?>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
<script type="text/javascript" src="../../includes/JQuery/jquery-1.7.2.min.js"></script>
<?php
$obrid = pegaObraID($preid);
echo cabecalhoObra($obrid);
monta_titulo('Análise de Terreno do Objeto', '');

$obSubacaoControle = new SubacaoControle();

$arDados = $obSubacaoControle->recuperarPreObra($preid);
$arDados = ($arDados) ? $arDados : array();

$boAtivo = 'N';
$stAtivo = 'disabled="disabled"';
$stObrigatorio = 'N';

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="subtitulodireita">Nome do terreno:</td>
	<td>
	<?php
	$predescricao = $arDados['predescricao'];
	echo campo_texto( "predescricao", $stObrigatorio, $boAtivo, '', 40, '', '', '','','','','id="predescricao"','',$predescricao);
	?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita">Tipo da Obra:</td>
	<td>
	<?php
	$sql = "SELECT ptoid AS codigo, ptodescricao AS descricao FROM obras.pretipoobra WHERE ptocategoria IS NOT NULL";
	$arTipoObra = $db->carregar($sql);
	$arTipoObra = $arTipo ? $arTipo : $arTipoObra;
	$ptoid = $arDados['ptoid'];
	$db->monta_combo( "ptoid", $arTipoObra, $boAtivo, 'Selecione...', '', '', '', '', $stObrigatorio, 'ptoid',false,$ptoid,'Tipo da Obra');
	?>
	</td>
</tr>
<tr>
	<td class="subtitulodireita">Unidade de Medida:</td>
	<td>Unidade Escolar</td>
</tr>
<?php
	$endereco = new Endereco();
	$entidade->enderecos[0] = $endereco;
?>
	<tr>
		<td align="left" colspan="2"><strong>Endereço do terreno</strong></td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap">
			<label>CEP:</label>
		</td>
		<td>
			<input type="text" name="endcep1" title="CEP" onkeyup="this.value=mascaraglobal('##.###-###', this.value);"
                               class="CampoEstilo" id="endcep1" value="<?php echo $arDados['precep']; ?>" size="13"
                               maxlength="10" <?php echo $stAtivo ?> />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Logradouro:</label>
		</td>
		<td>
			<input type="text" title="Logradouro" name="endlog" class="CampoEstilo" id="endlog1"
                               value="<?php echo $arDados['prelogradouro']; ?>" size="48" <?php echo $stAtivo ?> />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Número:</label>
		</td>
		<td>
			<input type="text" name="endnum" title="Número" class="CampoEstilo" id="endnum1"
                               value="<?php echo $arDados['prenumero']; ?>" size="6" maxlength="4"
                               onkeypress="return somenteNumeros(event);" <?php echo $stAtivo ?> />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Complemento:</label>
		</td>
		<td>
			<input type="text" name="endcom" class="CampoEstilo" id="endcom1"
                               value="<?php echo $arDados['precomplemento']; ?>" size="48"
                               maxlength="100" <?php echo $stAtivo ?> />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Bairro:</label>
		</td>
		<td>
			<input type="text" title="Bairro" name="endbai" class="CampoEstilo" id="endbai1"
                               value="<?php echo $arDados['prebairro']; ?>" <?php echo $stAtivo ?> />
		</td>
	</tr>

	<tr id="tr_estado">
		<td class="subtitulodireita">Estado:</td>
		<td>
			<?php
				$estuf = $arDados['estuf'];

				if($_SESSION['par']['estuf']){
					$where = " where e.estuf = '{$_SESSION['par']['estuf']}' ";
				}

				$sql = "
					Select	e.estuf as codigo,
							e.estdescricao as descricao
					from territorios.estado e
					$where
					order by  e.estdescricao asc
				";
				$db->monta_combo( "estuf", $sql, $boAtivo, 'Selecione...', 'filtraTipo', '', '', '', $stObrigatorio, 'estuf1',false,$estuf,'Estado');
			?>
		</td>
	</tr>
	<tr id="tr_municipio">
		<td class="subtitulodireita">Município:<br/></td>
		<td id="municipio">
			<?php
				if ($arDados['estuf']) {
					$sql = "select
								 muncod as codigo,
								 mundescricao as descricao
								from
								 territorios.municipio
								where
								 estuf = '".$arDados['estuf']."'
								order by
								 mundescricao asc";
					$muncod_ = $arDados['muncod'];
					$db->monta_combo( "muncod_", $sql, $boAtivo, 'Selecione o Estado', '', '', '','',$stObrigatorio, 'muncod_',false,$muncod_,'Município');
				} else {
					$db->monta_combo( "muncod_", array(), $boAtivo, 'Selecione o Estado', '', '', '', '', $stObrigatorio, 'muncod_',false,null,'Município');
				}
			?>
		</td>
	</tr>
	<script> document.getElementById('endcep1').value = mascaraglobal('##.###-###', document.getElementById('endcep1').value);</script>
	<?php
		$latitude = explode('.',$arDados['prelatitude']);
		$longitude = explode('.',$arDados['prelongitude']);
	?>
	<tr>
		<td class="SubTituloDireita">Latitude :</td>
		<td>
			<input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden">
				<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> º
			<input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden">
				<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  '
			<input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden">
				<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> "
			<input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden">
				<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude :</td>
		<td>
			<input name="longitude[]" id="graulongitude1" maxlength="2" size="3" value="<? echo $longitude[0]; ?>" type="hidden">
				<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	º
			<input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden">
				<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  '
			<input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden">
				<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> "
			<input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden">
				<span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
			<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">&nbsp;</td>
		<td>
			<a href="#" onclick="abreMapaEntidade('1');">Visualizar / Buscar No Mapa</a>
			<input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
		</td>
	</tr>

</table>

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td colspan="2" class="SubTituloCentro">
                <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
                <script type="text/javascript" src="../includes/remedial.js"></script>
                <script type="text/javascript" src="../includes/superTitle.js"></script>

                <?
                $sql = "SELECT
                                        arqnome, arq.arqid,
                                        arq.arqextensao, arq.arqtipo,
                                        arq.arqdescricao,
                                        to_char(arq.arqdata, 'DD/MM/YYYY') as data,
                                        arc.arqvalidacao
                                        {$excluir}
                                FROM
                                        public.arquivo arq
                                LEFT JOIN
                                        public.arquivo_recuperado arc ON arc.arqid = arq.arqid
                                INNER JOIN
                                        obras.preobrafotos pof ON arq.arqid = pof.arqid
                                INNER JOIN
                                        obras.preobra pre ON pre.preid = pof.preid
                                WHERE
                                        pre.preid = {$preid}
                                AND
                                        (substring(arqtipo,1,5) = 'image')
                                ORDER BY
                                        arq.arqid";
                                //LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
                $fotos = ($db->carregar($sql));
                $_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");

                if ($fotos) {
                    $_SESSION['imgparams'] = array(
                        "filtro" => "cnt.preid={$preid}",
                        "tabela" => "obras.preobrafotos"
                    );
                    for ($k=0; $k < count($fotos); $k++) {
                        echo <<<HTML
<br />
<div style="width:850px;text-align:center;margin:2px auto">
    <img title="{$fotos[$k]["arqdescricao"]}" id="{$fotos[$k]["arqid"]}" border="1px"
        src="../../slideshow/slideshow/verimagem.php?newwidth=640&newheight=480&_sisarquivo=obras&arqid={$fotos[$k]["arqid"]}"
        width="640" height="480" /><br />
        {$fotos[$k]["arqdescricao"]} - {$fotos[$k]["data"]}<br/>
    {$fotos[$k]["acao"]}
</div>
HTML;
                    }
                } else {
                    echo "Não existem fotos cadastradas";
                }
                ?>
            <br />
        </td>
    </tr>
</table>
<script type="text/javascript" language="javascript">
function abreMapaEntidade(tipoendereco){
	var graulatitude = window.document.getElementById("graulatitude"+tipoendereco).value;
	var minlatitude  = window.document.getElementById("minlatitude"+tipoendereco).value;
	var seglatitude  = window.document.getElementById("seglatitude"+tipoendereco).value;
	var pololatitude = window.document.getElementById("pololatitude"+tipoendereco).value;

	var graulongitude = window.document.getElementById("graulongitude"+tipoendereco).value;
	var minlongitude  = window.document.getElementById("minlongitude"+tipoendereco).value;
	var seglongitude  = window.document.getElementById("seglongitude"+tipoendereco).value;

	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	var janela=window.open('../../apigoogle/php/mapa_padraon.php?tipoendereco='+tipoendereco+'&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude, 'mapa','height=650,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();
}
</script>
