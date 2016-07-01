<?php

class generatorForm
{

    public function generate($schema, $table, $entity)
    {

        $bodyHtml = '<?php 
//    require_once "config.inc";
//    include APPRAIZ . "includes/classes_simec.inc";
//    include APPRAIZ . "includes/funcoes.inc";

    $db = new cls_banco();
    
    //Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";

    monta_titulo($titulo_modulo, \'&nbsp;\');


    if(isset($_POST[\'action\']) && $_POST[\'action\'] == \'save\'){
    
        include APPRAIZ . "www/gerador/Model.php";
        include APPRAIZ . "www/gerador/classes/'.$schema.'/models/'.ucfirst($table).'.php";

        $model' . ucfirst($table) . ' = new Model_' . ucfirst($table) . '();
        $model' . ucfirst($table) . '->populateEntity($_POST);
        $model' . ucfirst($table) . '->save();
        
        ver($_POST, $model' . ucfirst($table) . '->error,d);
    }

?>';

        $formHtml = $this->formHtml($entity, $schema, $table);

        $html = $bodyHtml . $formHtml;

        $files = APPRAIZ . "www/gerador/classes/";
        if (!file_exists($files))
        {
            if (mkdir($files, 0777))
            {
                $msg[] = array('status' => '1', 'msg' => 'Pasta de arquivos criada com sucesso!');
            } else
            {
                $msg[] = array('status' => '0', 'msg' => 'Não pode ser criado a pasta de arquivos!');
            }
        }

        $schemaFile = $files . $schema;
        if (!file_exists($schemaFile))
        {
            if (mkdir($schemaFile, 0777))
            {
                $msg[] = array('status' => '1', 'msg' => "Pasta {$schema} criada com sucesso!");
            } else
            {
                $msg[] = array('status' => '0', 'msg' => "Não pode ser criado a pasta '{$schema}'!");
            }
        }

        $formsFile = $schemaFile . "/forms/";
        if (!file_exists($formsFile))
        {
            if (mkdir($formsFile, 0777))
            {
                $msg[] = array('status' => '1', 'msg' => "Pasta 'forms' criada com sucesso!");
            } else
            {
                $msg[] = array('status' => '0', 'msg' => "Não pode ser criado a pasta 'models'!");
            }
        }

        $formPathFile = $formsFile . ucFirst($table) . '.php';
        if ($formFile = fopen($formPathFile, "w+"))
        {
            $msg[] = array('status' => '1', 'msg' => "Arquivo {$table}.php criada com sucesso!");
//            $teste = chmod($tablePathFile, '777');
            // Dando permissao no arquivo
            exec("chmod -R 777 $formPathFile");
        } else
        {
            $msg[] = array('status' => '0', 'msg' => "Não pode ser criado o arquivo '{$table}.php'!");
        }


        fwrite($formFile, $html);

//        ver($schemaFile, $formFile, $table, $entity);

//        echo '<b>Schema:</b> ' . $schema;
//        echo '<br />';
//        echo '<b>Tabela:</b> ' . $table;
//        echo '<br />';
//        echo '<b>Caminho dos arquivos:</b> ' . $schemaFile;
//        echo '<br />';
//        echo '<b>Caminho da pasta Form:</b> ' . $formPathFile;
//        echo '<br />';
        include_once($formPathFile);
        ver($schema , $table , $schemaFile , $formPathFile ,$msg);
//

//        echo $html;
        exit;
    }

//    echo $db->monta_combo("seriehistorica",$sql,'S',"","","","","100","S","","",$seriehistorica);
    public function formHtml($entity, $schema, $table)
    {
        $db = new cls_banco();
        
        $html = '
<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>
<form action="" method="post" name="form_save" id="form_save">
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <input type="hidden" name="action" value="save"/>
    ';
?>
        <?php
        foreach ($entity as $column) {
//            ver($entity,d);

            if(($column['is_nullable'] == 'NO')){
                $required = 'required="required"';
                $booRequired = 'S';
                
            } else {
                $required = '';
                $booRequired = 'N';
            }
//            if($schema == 'pes'){
//                $label = ucfirst(substr($column['column_name'], 3));
//            } else {
                $label = ucfirst($column['column_name']);
//            }
            $element = '';
        ?>
            <?php

//            if(strstr($column['constraint_name'], 'fk')){
                
//                $fk = str_replace('fk', 'pk', $column['constraint_name']);
                
//                $sql = "
//                        SELECT 
//                                 kcu.table_schema as schema , kcu.table_name as table
//                        FROM information_schema.columns col
//                        --LEFT JOIN information_schema.tables tab
//                        --	ON col.table_schema = tab.table_schema
//                        --	AND col.table_name = tab.table_name
//                        LEFT JOIN information_schema.table_constraints tc
//                                ON tc.table_catalog = col.table_catalog
//                                AND tc.table_schema = col.table_schema
//                                --AND tc.table_name = col.table_name
//                                AND tc.constraint_type = 'PRIMARY KEY'
//                        LEFT JOIN information_schema.key_column_usage kcu
//                                ON kcu.table_catalog = tc.table_catalog
//                                AND kcu.table_schema = tc.table_schema
//                                AND kcu.table_name = tc.table_name
//                                --AND kcu.constraint_name = tc.constraint_name
//                                AND kcu.column_name = col.column_name
//                        WHERE col.table_schema = '{$schema}'
//                        AND col.table_name = '{$table}'
//                        AND kcu.constraint_name = '{$fk}'
//                        ";
//                $tableFk = $db->pegaLinha($sql);
                
                
//                $column['constraint_name'] = 'fk';
//                ver($tableFk,$sql,d);
//            } else {
            
                switch ($column['data_type']) {
                    case 'integer':
                        if (strstr($column['constraint_name'], 'pk'))
                        {
                            $label = false;
                            $element = '<input type="hidden" name="' . $column['column_name'] . '" id="' . $column['column_name'] . '" />';
                        } else
                        {
                            $max = ($column['character_maximum_length']) ? $column['character_maximum_length'] : 30;
    //                        $element = campo_texto($column['column_name'], ($required)? 'S' : 'N', 'S', 'Descrição', 25, $max, '', '', '', '', '', 'id="'.$column['column_name'].'" ' . $required , '', '');
                            $element = '<?php echo campo_texto(\'' . $column['column_name'] . '\', \'' . $booRequired . '\' , \'S\', \'Descrição\' , 25, \'' . $max . '\', \'\', \'\', \'\', \'\', \'\', \'id="' . $column['column_name'] . '" ' . $required . '\') ?>';
                        }
                        break;
                    case 'smallint':
                        $element = '<?php echo campo_texto(\'' . $column['column_name'] . '\', \'' . $booRequired . '\' , \'S\', \'Descrição\' , 25, \'' . $column['character_maximum_length'] . '\', \'\', \'\', \'\', \'\', \'\', \'id="' . $column['column_name'] . '" ' . $required . '\') ?>';
                        break;
                    case 'character':
                        $element = '<?php echo campo_texto(\'' . $column['column_name'] . '\', \'' . $booRequired . '\' , \'S\', \'Descrição\' , 25, \'' . $column['character_maximum_length'] . '\', \'\', \'\', \'\', \'\', \'\', \'id="' . $column['column_name'] . '" ' . $required . '\') ?>';
                        break;
                    case 'character varying':
                        $element = '<?php echo campo_texto(\'' . $column['column_name'] . '\', \'' . $booRequired . '\' , \'S\', \'Descrição\' , 25, \'' . $column['character_maximum_length'] . '\', \'\', \'\', \'\', \'\', \'\', \'id="' . $column['column_name'] . '" ' . $required . '\') ?>';
                        break;
                    case 'numeric':
                        $element = '<?php echo campo_texto(\'' . $column['column_name'] . '\', \'' . $booRequired . '\' , \'S\', \'Descrição\' , 25, \'' . $column['character_maximum_length'] . '\', \'\', \'\', \'\', \'\', \'\', \'id="' . $column['column_name'] . '" ' . $required . '\') ?>';
                        break;
                    case 'date':
                        $element = '<?php echo campo_texto(\'' . $column['column_name'] . '\', \'' . $booRequired . '\' , \'S\', \'Descrição\' , 8, \'10\', \'##/##/####\', \'\', \'\', \'\', \'\', \'id="' . $column['column_name'] . '" ' . $required . '\') ?>';
                        break;
                    case 'boolean':
    //                    $element = campo_radio_sim_nao($column['column_name'], ($required)? 'S' : 'N', 'S', 'Descrição', 8, '10', '##/##/####', '', '', '', '', ' id="'.$column['column_name'].'" '  . $required , '', '');
                        $element = "<input type=\"radio\" name=\"{$column['column_name']}\" value=\"true\" /> Sim ";
                        $element .= "<input type=\"radio\" name=\"{$column['column_name']}\" value=\"false\" checked=\"true\" /> Não ";
                        break;

                    case 'text':
                        $element = "<?php echo campo_textarea('{$column['column_name']}', '{$booRequired}', 'S', '', 50, 5, 5000, null, null, null, null, null, null); ?>";
                        break;

                    default:

                        ver($column['data_type'], d);
                        
                        break;
                }
//            }
            if ($label)
            {
                $html .= '
    <tr class="exibir-info">
        <td align=\'right\' class="SubTituloDireita" style="vertical-align:top; width:25%; background-color: #cccccc;">
            ' . $label . '
        </td>
        <td>
            ' . $element . '
        </td>
    </tr>';
            } else
            {
                $html .= $element;
            }
        }

        $html .= '<tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td align="right" style="vertical-align:top; width:25%;">&nbsp;</td>
            <td>
                <input type="button" name="button_save" id="button_save" value="Salvar" />
            </td>
        </tr>
    </table>
</form>
<script lang="javascript">
    $.datepicker.regional[ \'pt-BR\' ];';
        foreach ($entity as $column) {
            if ($column['data_type'] == 'date')
            {
                $html .= '$("#' . $column['column_name'] . '").datepicker();';
            }
        };

$html .= '
    $(\'#button_save\').click(function(){
        if($.isValid()){
            $(\'#form_save\').submit();
        }
    });
</script>    
';

//        $html = ob_get_contents();
//        ob_clean();
//        $html = ob_get_contents();
//        echo $html;
//        $html = str_replace("../imagens/", "http://" . $_SERVER['HTTP_HOST'] . "/imagens/", $html);
//        ob_clean();
        return $html;
    }

}