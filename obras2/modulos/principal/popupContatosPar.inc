<?php

require_once APPRAIZ . "includes/classes/entidades.class.inc";

function recuperaTipoEntidade($dutid)
{
    global $db;
    return $db->pegaUm("SELECT dutdescricao FROM par.dadosunidadetipo WHERE dutid = {$dutid};");
}

function recuperaEntidadePorTipo($dutid, $estuf = null, $muncod = null)
{
    global $db;

    $where = '';
    if(!empty($estuf))
        $where = "iu.estuf = '{$estuf}'";
    else
        $where = "iu.muncod = '{$muncod}'";

    $sql = "select e.entid
            from par.entidade  e
            inner join par.dadosunidadetipo dt on dt.dutid = e.dutid
            inner join par.instrumentounidade iu on iu.inuid = e.inuid
            left join  territorios.municipio m on m.muncod = iu.muncod
            where entstatus = 'A' and {$where} and dt.dutid = {$dutid}
            ";

    return $db->pegaUm($sql);
}

verificaSessao('obra');

$esfera = $db->pegaUm( "select empesfera from obras2.empreendimento where empid = ".$_SESSION['obras2']['empid'] );

$dutidEst = '6';
if( $esfera == 'E' ) $dutidEst = '9';

$dutid = $_REQUEST['dutid'] ? $_REQUEST['dutid'] : $dutidEst;

$stFuncaoEntidade = recuperaTipoEntidade($dutid);

print carregaAbaDadosUnidade("/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&dutid=".$dutid.'&obrid='.$_REQUEST['obrid'], $_REQUEST['obrid'], $esfera);

monta_titulo( "Dados da unidade", "{$stFuncaoEntidade}" );

$sql = "select m.muncod, m.mundescricao, m.estuf, iu.inuid
		from obras2.obras o
			inner join entidade.endereco e on e.endid = o.endid
		    inner join territorios.municipio m on m.muncod = e.muncod
		    inner join par.instrumentounidade iu on iu.muncod = m.muncod
		where o.obrid = {$_REQUEST['obrid']}";

$dadosObras = $db->pegaLinha($sql);
$muncod = $dadosObras['muncod'];
$estuf = $dadosObras['estuf'];

$entid = ( $esfera == 'E' ) ? recuperaEntidadePorTipo($dutid, $estuf) : recuperaEntidadePorTipo($dutid, null, $muncod);

?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 22px">
			<?=$dadosObras['estuf'].' - '.$dadosObras['mundescricao'] ?>
		</td>
	</tr>
</table>
<?

if(empty($entid)){
    ?>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tblentidade">
    <tbody>
    <tr id="tr_titulo"><td class="SubTituloCentro" colspan="2"><?=str_to_upper($stFuncaoEntidade)?> não cadastrada</td></tr>
    </tbody>
    </table>
    <?
    die();
}

$sql = "select e.* from par.entidade e WHERE e.entid = {$entid}";
$entidade = $db->pegaLinha($sql);
$tipo = ($dutid == 7 || $dutid ==  2 || $dutid == 10) ? 'F' : 'P';
$entidades = new Entidades();

$entidades->mascaraglobal($entidade['entnumcpfcnpj'],"###.###.###-##")
?>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tblentidade">
    <tbody>
    <tr id="tr_titulo"><td class="SubTituloCentro" colspan="2"><?=str_to_upper($stFuncaoEntidade)?></td></tr>
    <? if ($tipo == 'F'): ?>
        <tr>
            <td class="SubTituloDireita">CPF :</td>
            <td><input readonly type="text" name="entnumcpfcnpj" id="entnumcpfcnpj" size="17" maxlength="14"
                       value="<?= $entidades->mascaraglobal($entidade['entnumcpfcnpj'], "###.###.###-##") ?>"
                       class="normal"</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome :</td>
            <td><input readonly type="text" name="entnome" id="entnome" size="45" maxlength="255"
                       value="<?= $entidade['entnome'] ?>" class="normal"/></td>
        </tr>
        <tr id="tr_entemail">
            <td class="SubTituloDireita">Email :</td>
            <td><input readonly type="text" name="entemail" id="entemail" size="45" maxlength="100"
                value="<?=$entidade['entemail'] ?>" class="normal" />
            </td>
        </tr>
        <tr id="tr_entnumrg">
            <td class="SubTituloDireita">Registro Geral (RG) :</td>
            <td><input readonly type="text" name="entnumrg" id="entnumrg" size="15" maxlength="15"
                value="<?=$entidade['entnumrg']?>" class="normal" />
            </td>
        </tr>
        <tr id="tr_entorgaoexpedidor">
            <td class="SubTituloDireita">Orgão Expedidor :</td>
            <td><input readonly type="text" name="entorgaoexpedidor" id="entorgaoexpedidor" size="15" maxlength="10"
                value="<?=$entidade['entorgaoexpedidor'] ?>" class="normal" />
            </td>
        </tr>
        <tr id="tr_entsexo">
            <td class="SubTituloDireita">Sexo :</td>
            <td><select disabled="disabled" name="entsexo" class="CampoEstilo" id="entsexo">
                <option value="">Selecione</option>
                <option value="M" <?= (($entidade['entsexo']=="M")?"selected":"") ?>>Masculino</option>
                <option value="F" <?= (($entidade['entsexo']=="F")?"selected":"") ?>>Feminino</option>
                </select>
            </td>
        </tr>
        <tr id="tr_entdatanasc">
            <td class="SubTituloDireita">Data de Nascimento :</td>
            <td><input readonly type="text" name="entdatanasc" id="entdatanasc" size="12" maxlength="10"
                value="<?= (($entidade['entdatanasc'] && $pc = explode("-", $entidade['entdatanasc'])) ? $pc[2] . "/" . $pc[1] . "/" . $pc[0] : "")?>" class="normal" />
            </td>
        </tr>
        <tr id="tr_entnumdddresidencial">
            <td class="SubTituloDireita">Telefone residencial :</td>
            <td>( <input readonly type="text" name="entnumdddresidencial" id="entnumdddresidencial" size="3"
                maxlength="2" value="<?=$entidade['entnumdddresidencial']?>" class="normal" /> )
                <input readonly type="text" name="entnumresidencial" id="entnumresidencial" size="11" maxlength="9"
                value="<?=$entidade['entnumresidencial']?>" class="normal" />
            </td>
        </tr>
        <tr id="tr_entnumdddcomercial">
            <td class="SubTituloDireita">Telefone comercial :</td>
            <td>( <input readonly type="text" name="entnumdddcomercial" id="entnumdddcomercial" size="3" maxlength="2"
                value="<?=$entidade['entnumdddcomercial']?>" class="normal" /> ) <input readonly type="text"
                name="entnumcomercial" id="entnumcomercial" size="11" maxlength="9" value="<?=$entidade['entnumcomercial']?>">
                <strong>Ramal :</strong> <input readonly type="text" name="entnumramalcomercial"
                id="entnumramalcomercial" size="5" maxlength="4" value="<?=$entidade['entnumramalcomercial']?>" class="normal" />
            </td>
        </tr>
        <tr id="tr_entnumdddfax">
            <td class="SubTituloDireita">Fax :</td>
            <td>( <input readonly type="text" name="entnumdddfax" id="entnumdddfax" size="3" maxlength="2"
                value="<?=$entidade['entnumdddfax']?>" class="normal" />
                <input readonly type="text" name="entnumfax" id="entnumfax" size="11" maxlength="9"
                value="<?=$entidade['entnumfax']?>" class="normal" />
                <strong>Ramal :</strong> <input readonly type="text" name="entnumramalfax" id="entnumramalfax" size="5"
                maxlength="4" value="<?=$entidade['entnumramalfax']?>" class="normal" />
            </td>
        </tr>
        <tr id="tr_entnumdddcelular">
            <td class="SubTituloDireita">Celular :</td>
            <td>( <input readonly type="text" name="entnumdddcelular" id="entnumdddcelular" size="3" maxlength="2"
                value="<?=$entidade['entnumdddcelular']?>" class="normal" />
                <input readonly type="text" name="entnumcelular" id="entnumcelular" size="12" maxlength="10"
                value="<?=$entidade['entnumcelular']?>" class="normal" />
            </td>
        </tr>
    <? else: ?>
        <? if($dutid != 8): ?>
        <tr id="tr_entnumcpfcnpj">
            <td class="SubTituloDireita">CNPJ :</td>
            <td><input readonly type="text" name="entnumcpfcnpj" id="entnumcpfcnpj" size="22" maxlength="18"
                       value="<?= $entidades->mascaraglobal($entidade['entnumcpfcnpj'], "##.###.###/####-##") ?> "
                       class="normal"/></td>
        </tr>
        <tr id="tr_entnuninsest">
            <td class="SubTituloDireita">Inscrição Estadual :</td>
            <td><input readonly type="text" name="entnuninsest" id="entnuninsest" size="16" maxlength="14"
                       value="<?= $entidade['entnuninsest'] ?>" class="normal"/></td>
        </tr>
        <? endif; ?>
        <tr id="tr_entnome">
            <td class="SubTituloDireita">Nome :</td>
            <td><input readonly type="text" name="entnome" id="entnome" size="90" maxlength="255"
                       value="<?= $entidade['entnome'] ?>"/></td>
        </tr>
        <? if($dutid != 8): ?>
        <tr id="tr_entrazaosocial">
            <td class="SubTituloDireita">Razão Social :</td>
            <td><input readonly type="text" name="entrazaosocial" id="entrazaosocial" size="90" maxlength="255"
                       value="<?= $entidade['entrazaosocial'] ?>"/></td>
        </tr>
        <? endif; ?>
        <tr id="tr_entemail">
            <td class="SubTituloDireita">Email :</td>
            <td><input readonly type="text" name="entemail" id="entemail" size="45" maxlength="100"
                       value="<?= $entidade['entemail'] ?>"/>
            </td>
        </tr>
        <tr id="tr_entsig">
            <td class="SubTituloDireita">Sigla :</td>
            <td><input readonly type="text" name="entsig" id="entsig" size="45" maxlength="50"
                value="<?= $entidade['entsig']?>" class="normal" ?>
            </td>
        </tr>
        <tr id="tr_entnumdddcomercial">
            <td class="SubTituloDireita">Telefone comercial :</td>
            <td>( <input readonly type="text" name="entnumdddcomercial" id="entnumdddcomercial" size="3" maxlength="2"
                         value="<?= $entidade['entnumdddcomercial'] ?>" class="normal"/> ) <input readonly type="text"
                                                                                                  name="entnumcomercial"
                                                                                                  id="entnumcomercial"
                                                                                                  size="11"
                                                                                                  maxlength="9"
                                                                                                  value="<?= $entidade['entnumcomercial'] ?>">
                <strong>Ramal :</strong> <input readonly type="text" name="entnumramalcomercial"
                                                id="entnumramalcomercial" size="5" maxlength="4"
                                                value="<?= $entidade['entnumramalcomercial'] ?>" class="normal"/>
            </td>
        </tr>
        <tr id="tr_entnumdddfax">
            <td class="SubTituloDireita">Fax :</td>
            <td>( <input readonly type="text" name="entnumdddfax" id="entnumdddfax" size="3" maxlength="2"
                         value="<?=$entidade['entnumdddfax']?>" class="normal" />
                <input readonly type="text" name="entnumfax" id="entnumfax" size="11" maxlength="9"
                       value="<?=$entidade['entnumfax']?>" class="normal" />
                <strong>Ramal :</strong> <input readonly type="text" name="entnumramalfax" id="entnumramalfax" size="5"
                                                maxlength="4" value="<?=$entidade['entnumramalfax']?>" class="normal" />
            </td>
        </tr>

        <?
        $sql = "SELECT m.*
                FROM par.entidade  e
                inner join territorios.municipio m on m.muncod = e.muncod
                WHERE e.entid = {$entidade['entid']}";
        $municipio = $db->pegaLinha($sql);
        ?>

        <tr id="tr_titulo">
            <td class="SubTituloCentro" colspan="2">ENDEREÇO</td>
        </tr>
        <tr id="tr_endcep">
            <td class="SubTituloDireita">CEP :</td>
            <td><input readonly type="text" name="endcep" id="endcep"
                       value="<?= $entidades->mascaraglobal($entidade['endcep'], "#####-###") ?>"/>
            </td>
        </tr>
        <tr id="tr_endlog">
            <td class="SubTituloDireita">Logradouro :</td>
            <td><input readonly type="text"
                name="endlog" id="endlog" size="80" maxlength="200" value="<?=$entidade['endlog']?>" />
            </td>
        </tr>
        <tr id="tr_endcom">
            <td class="SubTituloDireita">Complemento :</td>
            <td><input readonly type="text"
                name="endcom" id="endcom" size="80" maxlength="200" value="<?=$entidade['endcom']?>"/>
            </td>
        </tr>
        <tr id="tr_endnum">
            <td class="SubTituloDireita">Número :</td>
            <td><input readonly type="text"
                name="endnum" id="endnum" size="11" maxlength="10" value="<?=$entidade['endnum']?>" />
            </td>
        </tr>
        <tr id="tr_endbai">
            <td class="SubTituloDireita">Bairro :</td>
            <td><input readonly type="text"
                name="endbai" id="endbai" size="80" maxlength="100" value="<?=$entidade['endbai']?>" />
            </td>
        </tr>
        <tr id="tr_estuf">
            <td class="SubTituloDireita">UF :</td>
            <td><input readonly type="text"
                name="estuf" id="estuf" size="3" maxlength="2" value="<?=$municipio['estuf']?>" />
            </td>
        </tr>
        <tr id="tr_mundescricao">
            <td class="SubTituloDireita">Munícipio :</td>
            <td><input readonly type="text" id="mundescricao" name="mundescricao" size="30" value="<?=$municipio['mundescricao']?>"/>
            </td>
        </tr>
        <tr id="tr_latitude">
            <td class="SubTituloDireita">Latitude :</td>
            <td>
                <?=$entidade['medlatitude']?>            </td>
        </tr>
        <tr id="tr_longitude">
            <td class="SubTituloDireita">Longitude :</td>
            <td>
                <?=$entidade['medlongitude']?>
            </td>
        </tr>
    <? endif; ?>
    </tbody>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
